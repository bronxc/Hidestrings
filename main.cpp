/*
	A program used for generating hidden strings using common
	encoding techniques.
*/

#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <random>
#include <stdio.h>
#include <time.h>

#define RCHAR 0xFF

#define ENCODE_CHAR(ch, key, offset) \
	( ( ch ^ key ) + offset )

#define DECODE_CHAR(ch, key, offset) \
	( (ch - offset) ^ key )

char * decode(char * buf, int length, unsigned char key, unsigned char offset)
{
	char * result = (char *)malloc(length);
	memset(result, length, 0);

	for (int i = 0; i < length; i++)
	{
		result[i] = DECODE_CHAR(buf[i], key, offset);
	}

	return result;
}

char * encode(char * buf, int length, unsigned char key, unsigned char offset)
{
	char * result = (char *)malloc(length);
	memset(result, length, 0);

	for (int i = 0; i < length; i++)
	{
		result[i] = ENCODE_CHAR(buf[i], key, offset);
	}

	return result;
}

typedef struct HIDESTRING{
	char * name;
	char * str;
	char * encoded_str;
	int len;
	unsigned char key, offset;

	HIDESTRING(char * _name, char * _str, unsigned char key, unsigned char offset)
	{
		this->name = (char *)malloc(strlen(_name) + 1);
		this->str = (char *)malloc(strlen(_str) + 1);

		this->len = strlen(_str);

		strncpy(this->name, _name, strlen(_name));
		this->name[strlen(_name)] = '\0';
		
		strncpy(this->str, _str, strlen(_str));
		this->str[strlen(_str)] = '\0';

		this->key = key;
		this->offset = offset;

		this->encoded_str = encode(this->str, this->len, key, offset);
	}

	~HIDESTRING()
	{
		free(this->name);
		free(this->str);
		free(this->encoded_str);
	}

	void print()
	{
		printf("// -- %s -- //\n", this->str);
		printf("#define %s ", this->name);
		printf("( decode(\\\n\t\"");
		for (int i = 0; i < this->len; i++)
		{
			if (i % 20 == 0 && i != 0)
			{
				printf("\"\\\n\t\"");
			}

			printf("\\x%02x", (unsigned char)(this->encoded_str[i]));
		}
		printf("\", %d, 0x%02X, 0x%02X) )\n", this->len, this->key, this->offset);
	}

} HIDESTRING;

unsigned char rand_char(unsigned char max)
{
	unsigned char r = max;

	// Loop until we get something that isn't 0 or max
	while ((r = rand() % max) == 0) continue;

	return r;
}

int main(int argc, char * argv[])
{
	srand((unsigned int)time(0));

	printf("#define DECODE_CHAR(ch, key, offset) \\\n");
	printf("\t( (ch - offset) ^ key )\n\n");

	printf("%s",
		"char * decode(char * buf, int length, unsigned char key, unsigned char offset)\n"
		"{\n"
		"	char * result = (char *)malloc(length + 1);\n"
		"	memset(result, length + 1, 0);\n"
		"	for (int i = 0; i < length; i++)\n"
		"	{\n"
		"		result[i] = DECODE_CHAR(buf[i], key, offset);\n"
		"	}\n"
		"	return result;\n"
		"}\n\n"
	);

	HIDESTRING strings[] = {
		HIDESTRING("LSB_TARGET_URL", "http://10.248.187.21/security.bmp", rand_char(RCHAR), rand_char(RCHAR)),
		HIDESTRING("LSB_TARGET_URL", "http://10.248.187.21/clear.bmp", rand_char(RCHAR), rand_char(RCHAR)),
		HIDESTRING("LSB_TARGET_URL", "http://10.248.187.21/ssl.bmp", rand_char(RCHAR), rand_char(RCHAR)),
		HIDESTRING("LSB_TARGET_URL", "http://ec2-54-85-174-185.compute-1.amazonaws.com/cat.bmp", rand_char(RCHAR), rand_char(RCHAR)),
		HIDESTRING("USER_AGENT", "Mozilla/5.0 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)", rand_char(RCHAR), rand_char(RCHAR))
	};

	for (int i = 0; i < (sizeof(strings) / sizeof(HIDESTRING)); i++)
	{
		strings[i].print();
		puts("\n"); // seperate all strings by a new line
	}

	return 0;
}
