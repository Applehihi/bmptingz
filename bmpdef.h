#include <stdint.h>

/*Struct*/

struct pixelarray {
	int32_t height;
	int32_t width;
	byte* pixelarray;
};

/*Typedefs*/

	/*byte*/
typedef uint8_t byte;


/*Functions*/

	/*Convert 32 bit int into little endian 4 byte array*/
void lendint32(byte** destination, int32_t number);
	/*Convert 16 bit int into little endian 2 byte array*/
void lendint16(byte** destination, int16_t number);
	/*One bit bitmap*/
void onebitbmp(char* filename, int32_t width, int32_t eight, int32_t hres, int32_t vres, byte* pixel_array);
