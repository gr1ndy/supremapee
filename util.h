#pragma once

namespace util {
	__forceinline Address copy( Address dst, Address src, size_t size ) {
		__movsb(
			dst.as<uint8_t*>( ),
			src.as<uint8_t*>( ),
			size
		);

		return dst;
	}

	// memset
	__forceinline Address set( Address dst, uint8_t val, size_t size ) {
		__stosb(
			dst.as<uint8_t*>( ),
			val,
			size
		);

		return dst;
	}

	template< typename o = void*, typename i = void* >
	__forceinline o force_cast( i in ) {
		union { i in; o out; }
		u = { in };
		return u.out;
	};

	template < typename t = Address >
	__forceinline static t get_method( Address this_ptr, size_t index ) {
		return ( t )this_ptr.to< t* >( )[ index ];
	}

	// get base ptr ( EBP (x86_32) / RBP (x86_64) ).
	__forceinline uintptr_t GetBasePointer( ) {
		return ( uintptr_t )_AddressOfReturnAddress( ) - sizeof( uintptr_t );
	}

	// wide -> multi-byte
	__forceinline std::string WideToMultiByte( const std::wstring &str ) {
		std::string ret;
		int         str_len;

		// check if not empty str
		if( str.empty( ) )
			return {};

		// count size
		str_len = g_winapi.WideCharToMultiByte( CP_UTF8, 0, str.data( ), ( int )str.size( ), 0, 0, 0, 0 );

		// setup return value
		ret = std::string( str_len, 0 );

		// final conversion
		g_winapi.WideCharToMultiByte( CP_UTF8, 0, str.data( ), ( int )str.size( ), &ret[ 0 ], str_len, 0, 0 );

		return ret;
	}

	// multi-byte -> wide
	__forceinline std::wstring MultiByteToWide( const std::string &str ) {
		std::wstring    ret;
		int		        str_len;

		// check if not empty str
		if( str.empty( ) )
			return {};

		// count size
		str_len = g_winapi.MultiByteToWideChar( CP_UTF8, 0, str.data( ), ( int )str.size( ), nullptr, 0 );

		// setup return value
		ret = std::wstring( str_len, 0 );

		// final conversion
		g_winapi.MultiByteToWideChar( CP_UTF8, 0, str.data( ), ( int )str.size( ), &ret[ 0 ], str_len );

		return ret;
	}


	__forceinline Address PatternScan(uintptr_t mod, const char* sig, size_t size)
	{
		// Credits: MarkHC, although slightly modified by DucaRii and also documented
		static auto pattern_to_byte = [](const char* pattern)
		{
			// Prerequisites
			auto bytes = std::vector<int>{};
			auto start = const_cast<char*>(pattern);
			auto end = const_cast<char*>(pattern) + strlen(pattern);

			// Convert signature into corresponding bytes
			for (auto current = start; current < end; ++current)
			{
				// Is current byte a wildcard? Simply ignore that that byte later
				if (*current == '?')
				{
					++current;

					// Check if following byte is also a wildcard
					if (*current == '?')
						++current;

					// Dummy byte
					bytes.push_back(-1);
				}
				else
				{
					// Convert character to byte on hexadecimal base
					bytes.push_back(strtoul(current, &current, 16));
				}
			}
			return bytes;
		};


		// The region where we will search for the byte sequence
		const auto image_size = size;

		// Check if the image is faulty
		if (!image_size)
			return {};

		// Convert IDA-Style signature to a byte sequence
		auto pattern_bytes = pattern_to_byte(sig);

		const auto image_bytes = reinterpret_cast<byte*>(mod);

		const auto signature_size = pattern_bytes.size();
		const auto signature_bytes = pattern_bytes.data();

		// Now loop through all bytes and check if the byte sequence matches
		for (auto i = 0ul; i < image_size - signature_size; ++i)
		{
			auto byte_sequence_found = true;

			// Go through all bytes from the signature and check if it matches
			for (auto j = 0ul; j < signature_size; ++j)
			{
				if (image_bytes[i + j] != signature_bytes[j] // Bytes don't match
					&& signature_bytes[j] != -1) // Byte isn't a wildcard either, WHAT THE HECK
				{
					byte_sequence_found = false;
					break;
				}
			}

			// All good, now return the right address
			if (byte_sequence_found)
				return Address(uintptr_t(&image_bytes[i]));
		}

		// Byte sequence wasn't found
		return {};
	}



	/*Credits: @hinnie on unknowncheats: https://www.unknowncheats.me/forum/counterstrike-global-offensive/375564-rtti-helper-easy-vtable-pointers.html */
	__forceinline std::string BytesToIDAPattern(byte* bytes, size_t size) {
		std::stringstream ida_pattern;
		ida_pattern << std::hex << std::setfill('0');
		for (size_t i = 0; i < size; i++)
		{
			const int32_t current_byte = bytes[i];
			if (current_byte != 255)
				ida_pattern << std::setw(2) << current_byte;
			else
				ida_pattern << std::setw(1) << XOR("?");

			if (i != size - 1)
				ida_pattern << XOR(" ");
		}

		return ida_pattern.str();
	}

	/*Credits: @hinnie on unknowncheats: https://www.unknowncheats.me/forum/counterstrike-global-offensive/375564-rtti-helper-easy-vtable-pointers.html */
	__forceinline std::vector<uintptr_t> GetXRefsTo(uintptr_t address, uintptr_t start, uintptr_t size) {
		std::vector<uintptr_t> xrefs = {};

		// Convert the address over to an IDA pattern string
		const std::string ida_pattern = BytesToIDAPattern((byte*)&address, 4);

		// Get the end of the section (in our case the end of the .rdata section)
		const uintptr_t end = start + size;
		while (start && start < end)
		{
			uintptr_t xref = (uintptr_t)PatternScan(start, ida_pattern.c_str(), size);

			// If the xref is 0 it means that there either were no xrefs, or there are no remaining xrefs.
			// So we should break out of our loop, otherwise it will keep on trying to look for xrefs.
			if (!xref)
				break;

			// We've found an xref, save it in the vector, and add 4 to start, so it wil now search for xrefs
			// from the previously found xref untill we're at the end of the section, or there aren't any xrefs left.
			xrefs.push_back(xref);
			start = xref + 4;
		}

		return xrefs;
	}

	/*Credits: @hinnie on unknowncheats: https://www.unknowncheats.me/forum/counterstrike-global-offensive/375564-rtti-helper-easy-vtable-pointers.html */
	__forceinline bool GetSectionInfo(uintptr_t base_address, const char* section_name, uintptr_t& section_start, uintptr_t& section_size) {
		const PIMAGE_DOS_HEADER dos_header = (PIMAGE_DOS_HEADER)base_address;
		if (dos_header->e_magic != IMAGE_DOS_SIGNATURE)
			return false;

		const PIMAGE_NT_HEADERS32 nt_headers = (PIMAGE_NT_HEADERS32)(base_address + dos_header->e_lfanew);
		if (nt_headers->Signature != IMAGE_NT_SIGNATURE)
			return false;

		PIMAGE_SECTION_HEADER section_header = IMAGE_FIRST_SECTION(nt_headers);
		uint16_t number_of_sections = nt_headers->FileHeader.NumberOfSections;
		while (number_of_sections > 0)
		{
			// If we're at the right section
			if (!strcmp(section_name, (const char*)section_header->Name))
			{
				section_start = base_address + section_header->VirtualAddress;
				section_size = section_header->SizeOfRawData;
				return true;
			}

			section_header++;
			number_of_sections--;
		}

		return false;
	}

	



};