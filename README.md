# Astraea ー Binary File Format Reverse Engineering Framework

## Abstract

**Astraea** aims to be a powerful and modular scripting language, it is designed to facilitate the reverse engineering and manipulation of binary file formats.


## Motivation

Almost everything is stored as data in computers, and as the number of existing software never ceases to grow we end up with an infinity of file formats,
each with its own purpose and pecularities. Those formats most of times do not have an open specification or are purposefully obfuscated,
sometimes making it hard for users to keep control of their own data, or even stranded with unsupported software.

In this document we propose an intermediate scripting language to deal with file formats,
to help ensuring the compatibility of software and the ownership of data by the end user.


## Description

Astraea-the-language follows common programming concepts, it allows the definition of data types and the implementation of functions to manipulate those data types.
The language will be interpreted in a virtual runtime mainly, but may also be transpiled into other programming languages native code.

As we define a new standard it will take time for the project to grow and support all the common workflows.


## Examples

Bellow is a possible implementation of the ZIP archive format using **Astraea**.

### ZIP archive file

```javascript
public.zip_archive :: module {
  _metadata :: struct {
    authors     :: "Aethelwerka Software";
    description :: "ZIP archive file";
    categories  :: { "Archive", "Data Compression" };
    file_masks  :: { "*.zip", "*.zipx" };
    signatures  :: { 0x504B, "PK" };
    version     :: { 1, 0, 0 };
    references  :: {
        "https://pkware.cachefly.net/webdocs/casestudies/APPNOTE.TXT"
    };
  };

  //--------------------------------------------------
  from public.ms_dos import DosDate, DosTime;

  // Define the structures used in ZIP files.

  // Enum used for compression format.
  CompressionType :: enum u16 {
    Stored    = 0,
    Shrunk    = 1,
    Reduced1  = 2,
    Reduced2  = 3,
    Reduced3  = 4,
    Reduced4  = 5,
    Imploded  = 6,
    Token     = 7,
    Deflate   = 8,
    Deflate64 = 9,
  };

  // Local file header.
  LocalFile :: struct {
    signature          : [4] char;  // 0x04034B50
    version_to_extract : u16;
    flags              : u16;
    compression_method : CompressionType;
    last_mod_file_time : DosTime; 
    last_mod_file_date : DosDate;
    crc32              : u32 << ._format = hex;
    compressed_size    : u32;
    uncompressed_size  : u32;
    filename_length    : u16;
    extra_length       : u16;
    filename           : string << ._lenght = filename_length;
    extra              : [..] u8 << ._lenght = extra_length;
    compressed_data    : [..] u8 << ._lenght = compressed_size;
  } << ._title = .filename, ._bgcolor = 0xFFB6B5;

  // Defines an entry in the directory table.
  CentralDirectoryEntry :: struct {
    signature           : [4] char;  // 0x02014B50
    version_made_by     : u16;
    version_to_extract  : u16;
    flags               : u16;
    compression_method  : CompressionType;
    last_mod_file_time  : DosTime;
    last_mod_file_date  : DosDate;
    crc32               : u32 << ._format = hex;
    compressed_size     : u32;
    uncompressed_size   : u32;
    filename_length     : u16;
    extra_length        : u16;
    comment_length      : u16;
    disk_number_start   : u16;
    internal_attributes : u16;
    external_attributes : u32;
    header_offset       : u32;
    filename            : string << ._lenght = filename_length;
    extrafield          : [..] u8 << ._lenght = extra_length;
    comment             : [..] u8 << ._lenght = comment_length;
  } << ._title = .filename, ._bgcolor = 0xFF77AC;

  // Defines the digital signature.
  DigitalSignature :: struct {
    signature   : [4] char;  // 0x05054B50
    data_length : u16;
    data        : [data_length] u8;
  };
      
  // Defines the Data descriptor.
  DataDescriptor :: struct {
    /** The signature field is optional in this structure. 
     * We may need to rethink the way this is declared.
     * Ideas: << (_optional / _maybe);  */
    signature         : [4] char;  // 0x08074B50
    crc32             : u32 << ._format = hex;
    compressed_size   : u32;
    uncompressed_size : u32;
  };

  // End of Central Directory Record (EOCD).
  EndOfCentralDirectory :: struct {
    signature           : [4] char;  // 0x06054B50
    disk_number         : u16;
    disk_number_start   : u16;
    dir_entries_on_disk : u16;
    dir_entries_total   : u16;
    directory_size      : u32;
    directory_offset    : u32;
    comment_length      : u16;
    comment             : string << ._lenght = comment_length;
  };

  // Zip-archives may contain the following structures.
  Zip :: struct {
    directories : [..] CentralDirectoryEntry;
    files       : [..] LocalFile;
    signatures  : [..] DigitalSignature;
    ending      : [..] EndOfCentralDirectory;
    descriptors : [..] DataDescriptor;
  };

  //--------------------------------------------------

  // Custom read function for Zip-archives.
  export
  read<Zip> :: function (fs : &File) {
    little_endian();
    // Read the zip file as a set of chunks
    while (!f.eof()) {
      tag := f.peek<u32>();  // Peek contents without advancing the file offset.
      // Read data depending upon tag - should start with 'PK'.
      match tag {
        0x02014B50 => directories : CentralDirectoryEntry;
        0x04034B50 => files       : LocalFile;
        0x05054B50 => signatures  : DigitalSignature;
        0x06054B50 => ending      : EndOfCentralDirectory;
        0x08074B50 => descriptors : DataDescriptor;
        none => error("Unknown ZIP tag encountered. Template stopped.");
      }
    }

    return Zip{directories, files, signatures, ending, descriptors};
  }

};  // public.zip-archive
```

As you may have noticed the ZIP implementation public.zip-archive imports some DOS definitions from the public.dos module, for the sake of completeness this module is defined bellow.

### DOS utilities

```javascript
public.ms_dos :: module {
  _metadata :: struct {
    authors     :: "Aethelwerka Software";
    description :: "Common DOS structures";
    categories  :: { "Utilities", "Operating System" };
    version     :: { 1, 0, 0 };
    references  :: {
        "docs.microsoft.com/en-us/windows/win32/sysinfo/ms-dos-date-and-time"
    };
  };
  
  //---------------------------------------------
  // Define common DOS structures.

  // DOS date and DOS time are packed 16-bit values
  // that specify the month, day, year and time of day a
  // DOS file was last written to.

  // The date format is packed as follows.
  export
  DosDate :: struct {
    day   : u5;  // Day of the month (1-31)
    month : u4;  // Month (1-12)
    year  : u7;  // Year offset from 1980.
  };

  // The time format is packed as follows.
  export
  DosTime :: struct {
    second : u5;  // Second divided by 2;
    minute : u6;  // Minute (0-59)
    hour   : u5;  // Hour (0-23 on ar 24-hour clock)
  };
};  // public.ms-dos
```

## ...

Work in progress.
