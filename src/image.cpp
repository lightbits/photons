#include "image.h"
#include <fstream>
#include <iostream>

//struct ResizeParam
//{
//	ResizeParam(const Image *original_, Image *result_, uint32 scale_, uint32 x0_, uint32 y0_, uint32 w_, uint32 h_)
//		: original(original_), result(result_), scale(scale_), x0(x0_), y0(y0_), w(w_), h(h_) { }
//	const Image *original;
//	Image *result;
//	uint32 scale;
//	uint32 x0;
//	uint32 y0;
//	uint32 w;
//	uint32 h;
//};
//
//void resizeBlock(const ResizeParam &param)
//{
//	for(uint32 y = param.y0; y < param.y0 + param.h; ++y)
//		for(uint32 x = param.x0; x < param.x0 + param.w; ++x)
//			param.result->setColor(x, y, param.original->getColor(x / param.scale, y / param.scale));
//}
//
//Image resize(const Image &image, uint32 scale)
//{
//	uint32 nw = image.getWidth() * scale;
//	uint32 nh = image.getHeight() * scale;
//	Image result(nw, nh);
//	result.clear(255, 255, 255);
//
//	const uint32 N = 2;
//	std::thread t[4];
//	uint32 dx = result.getWidth() / N;
//	uint32 dy = result.getHeight() / N;
//	for(uint32 i = 0; i < N * N; ++i)
//		t[i] = std::thread(resizeBlock, ResizeParam(&image, &result, scale, dx * (i % N), dy * (i / N), dx, dy));
//
//	for(uint32 i = 0; i < N * N; ++i)
//		t[i].join();
//
//	return result;
//}

Image::Image(uint32 width, uint32 height)
{
	this->width = width;
	this->height = height;
	data = new uint8[width * height * 3];
}

Image::~Image()
{
	delete[] data;
}

Image::Image(const Image &copy)
{
	this->width = copy.width;
	this->height = copy.height;
	data = new uint8[width * height * 3];
	std::copy(copy.data, copy.data + width * height * 3, data);
}

Image Image::operator=(const Image &rhs)
{
	width = rhs.width;
	height = rhs.height;
	delete[] data;
	data = new uint8[width * height * 3];
	std::copy(rhs.data, rhs.data + width * height * 3, data);
	return *this;
}

void Image::setColor(uint32 x, uint32 y, uint8 r, uint8 g, uint8 b)
{
	if(x >= width || y >= height)
		return;

	unsigned int offset = 3 * (x + width * y);
	data[offset + 0] = r;
	data[offset + 1] = g;
	data[offset + 2] = b;
}

void Image::setColor(uint32 x, uint32 y, const Color &color)
{
	setColor(x, y, color.r, color.g, color.b);
}

void Image::setColor(uint32 x, uint32 y, const Colorf &color)
{
	setColor(x, y, uint8(color.r * 255), uint8(color.g * 255), uint8(color.b * 255));
}

void Image::clear(uint8 r, uint8 g, uint8 b)
{
	for(uint32 y = 0; y < height; ++y)
		for(uint32 x = 0; x < width; ++x)
			setColor(x, y, r, g, b);
}

Color Image::getColor(uint32 x, uint32 y) const
{
	if(x >= width || y >= height)
		return Color(0, 0, 0);

	uint32 offset = 3 * (y * width + x);
	return Color(data[offset + 0], data[offset + 1], data[offset + 2]);
}

Colorf Image::getColorf(uint32 x, uint32 y) const
{
	if(x >= width || y >= height)
		return Colorf(0, 0, 0);

	uint32 offset = 3 * (y * width + x);
	return Colorf(data[offset + 0] / 255.0f, data[offset + 1] / 255.0f, data[offset + 2] / 255.0f);
}

static void writeChar( std::ostream &stream, char val ) {
	stream.write( &val, 1 );
}

static void writeShort( std::ostream &stream, short val ) {
	stream.write( ( char * ) &val, 2 );
}

static void writeSignedInt( std::ostream &stream, signed int val ) {
	stream.write( ( char * ) &val, 4 );
}

static void writeUnsignedInt( std::ostream &stream, unsigned int val ) {
	stream.write( ( char * ) &val, 4 );
}

static char readChar( std::istream &stream ) {
	char val = 0;
	stream.read( &val, 1 );
	return val;
}

static unsigned char readUnsignedChar( std::istream &stream ) {
	unsigned char val = 0;
	stream.read( ( char * )&val, 1 );
	return val;
}

static short readShort( std::istream &stream ) {
	short val = 0;
	stream.read( ( char * ) &val, 2 );
	return val;
}

static signed int readSignedInt( std::istream &stream ) {
	signed int val = 0;
	stream.read( ( char * ) &val, 4 );
	return val;
}

static unsigned int readUnsignedInt( std::istream &stream ) {
	unsigned int val = 0;
	stream.read( ( char * ) &val, 4 );
	return val;
}

// BMP-header-constants:
static const short BITS_PER_PIXEL = 24;
static const int BYTES_PER_PIXEL = BITS_PER_PIXEL / 8;

static const int BMPHEADER_SIZE = 54;
// Header
static const char HEADER1 = 'B';
static const char HEADER2 = 'M';
// DIB
static const unsigned int DIB_SIZE = 40; // Should be 40
static const short int COLOR_PLANES = 1; // 1

int rowSize( int width, int bpp ) {
	// http://en.wikipedia.org/wiki/BMP_file_format#Pixel_storage
	return ( ( bpp * width + 31 ) / 32 ) * 4;
}

bool Image::load(const std::string &filename) {
	std::fstream file;
	file.open( filename.c_str(), std::ios::in | std::ios::binary );

	if ( !file ) {
		std::cerr << "Failed to open file: " << filename << std::endl;
		return false;
	}
	if ( !( readChar( file ) == HEADER1 ) ) {
		file.close();
		std::cerr << "Failed reading BMP-header: No 'B' " << std::endl;
		return false;
	}
	if ( !( readChar( file ) == HEADER2 ) ) {
		file.close();
		std::cerr << "Failed reading BMP-header: No 'M' " << std::endl;
		return false;
	}
	unsigned int fileSize = readUnsignedInt( file );  // size (bmpheader + (row_size * imageHeight))
	readUnsignedInt( file );  // Reserved
	unsigned int offset = readUnsignedInt( file );
	if ( !( readUnsignedInt( file ) == DIB_SIZE ) ) {   // dib_size
		file.close();
		std::cerr << "Failed reading BMP-header: Invalid DIB-size " << std::endl;
		return false;
	}
	int imageWidth = readUnsignedInt( file );
	int imageHeight = readUnsignedInt( file );
	Image *image = new Image( imageWidth, imageHeight );
	unsigned char *data = image->getData();

	readShort( file );  // color_planes
	short bpp = readShort( file );
	if ( !( bpp == BITS_PER_PIXEL ) && !( bpp == 8 ) ) { // bits per pixel
		file.close();
		delete image;
		std::cerr << "Failed reading BMP-header: Unsupported bpp " << bpp << std::endl;
		return false;
	}
	if ( readUnsignedInt( file ) != 0 ) {  // Compression_method
		file.close();
		delete image;
		std::cerr << "Failed reading BMP-header: Compression not supported" << std::endl;
		return false;
	}
	unsigned int imageSize = readUnsignedInt( file );
	if ( imageSize != ( rowSize( image->getWidth(), bpp ) * image->getHeight() )  && imageSize != 0 ) {   // imageSize;
		file.close();
		delete image;
		std::cerr << "Failed reading BMP-header: Invalid image-size " << imageSize << std::endl;
		return false;
	}
	readSignedInt( file );  // horizontal_res
	readSignedInt( file );  // vertical_res
	unsigned int paletteColors = readUnsignedInt( file );  // palette_colors
	readUnsignedInt( file );  // important_colors

	if ( paletteColors == 0 && bpp == 8 ) {
		// 0 in a 8bpp image means 2^n colors, hardcode for 8bpp, as that is the only non 24bpp we support.
		paletteColors = 256;
	}

	// Additional sanity-checks:
	bool failure = false;
	// Verify that the reported fileSize is atleast as large as the image should be.
	// BMPs are apparently allowed to be larger than the usefull data they contain.
	if ( fileSize < ( offset + ( rowSize( image->getWidth(), bpp ) * imageHeight ) ) )  {
		std::cerr << "Failed reading BMP-header: Image size mismatch" << std::endl;
		failure = true;
	}

	if ( failure ) {
		delete image;
		file.close();
		return false;
	}

	// http://en.wikipedia.org/wiki/BMP_file_format#Pixel_storage
	int imagePitch = image->getHeight() * BYTES_PER_PIXEL;
	int padBytes = rowSize( image->getWidth(), bpp ) - ( image->getWidth() * BYTES_PER_PIXEL );

	if ( bpp == 8 ) { // Indexed, color-mapped image
		// Read palette:
		unsigned char *palette = new unsigned char[paletteColors * 4];
		// Only works on Little-Endian-machines
		for ( unsigned int i = 0; i < paletteColors * 4; i++ ) {
			palette[i] = readUnsignedChar( file );
		}
		file.seekg( offset, std::ios::beg );
		for ( unsigned int y = 0; y < image->getHeight(); y++ ) {
			for ( unsigned  int x = 0; x < image->getWidth(); x++ ) {
				if ( !file.good() ) {
					file.close();
					delete image;
					delete[] palette;
					std::cerr << "Failed reading BMP: Error while reading image data" << std::endl;
					return NULL;
				}
				unsigned int index = readUnsignedChar( file );
				if ( index > paletteColors ) {
					std::cerr << "Failed reading BMP: Indexed color out of range" << std::endl;
					delete image;
					file.close();
					return false;
				}
				Color c( palette[index * 4 + 0], palette[index * 4 + 1], palette[index * 4 + 2] );
				image->setColor( x, y, c );
			}
			if ( padBytes ) {
				file.ignore( padBytes );
			}

		}
		delete[] palette;
	} else if ( bpp == 24 ) {
		file.seekg( offset, std::ios::beg );
		for ( int y = 0; y < image->getHeight(); y++ ) {
			if ( !file.good() ) {
				file.close();
				delete image;
				std::cerr << "Failed reading BMP: Error while reading image data at line" << y << std::endl;
				return false;
			}
			for ( unsigned int x = 0; x < image->getWidth(); x++ ) {
				Color pixel;
				pixel.b = readUnsignedChar( file );
				pixel.g = readUnsignedChar( file );
				pixel.r = readUnsignedChar( file );
				image->setColor( x, y, pixel );
			}

			// Rows in BMPs are padded to power-of-four lengths.
			if ( padBytes ) {
				file.ignore( padBytes );
			}
		}
	}

	file.close();
	// TODO: Differing sizes?

	*this = *image;
	delete image;
	return true;
}

int Image::save(const std::string &filename ) {
	std::fstream file;
	file.open( filename.c_str(), std::ios::out | std::ios::binary );
	if ( !file.is_open() ) {
		return -1;
	}

	// Write Header:
	writeChar( file, HEADER1 );
	writeChar( file, HEADER2 );
	writeUnsignedInt( file, BMPHEADER_SIZE + rowSize(width, BITS_PER_PIXEL ) * height );   // size
	writeUnsignedInt( file, 0 );  // reserved
	writeUnsignedInt( file, BMPHEADER_SIZE );  //offset;
	// DIB
	writeUnsignedInt( file, DIB_SIZE );
	writeUnsignedInt( file, width );
	writeUnsignedInt( file, height );

	writeShort( file, COLOR_PLANES );  // color_planes
	writeShort( file, BITS_PER_PIXEL );
	writeUnsignedInt( file, 0 );  // compression_method
	writeUnsignedInt( file, rowSize( width, BITS_PER_PIXEL ) * height );   // imageSize
	writeSignedInt( file, 0 );  // horizontal_res
	writeSignedInt( file, 0 );  // vertical_res
	writeUnsignedInt( file, 0 );  // palette_colors
	writeUnsignedInt( file, 0 );  // important_colors

	// Write Actual data:

	// http://en.wikipedia.org/wiki/BMP_file_format#Pixel_storage
	int imagePitch = width * BYTES_PER_PIXEL;
	int padBytes = rowSize( width, BITS_PER_PIXEL ) - ( width * BYTES_PER_PIXEL );

	// BMPs have 0,0 in lower-left corner, so flip.
	for ( int y = 0; y < height; y++ ) {
		if ( !file.good() ) {
			file.close();
			std::cerr << "Failed writing BMP: Error while writing Image data" << std::endl;
			return -1;
		}
		for ( unsigned int x = 0; x < width; x++ ) {
			writeChar( file, data[(y * width + x) * BYTES_PER_PIXEL + 2] );
			writeChar( file, data[(y * width + x) * BYTES_PER_PIXEL + 1] );
			writeChar( file, data[(y * width + x) * BYTES_PER_PIXEL + 0] );
		}

		// Rows in BMPs are padded to power-of-four lengths.
		if ( padBytes ) {
			for ( int j = 0; j < padBytes; j++ ) {
				file.put( 0 );
			}
		}
	}
	file.close();
	return 0;
}