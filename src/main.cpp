#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <string>
#include <iostream>

int main()
{
	int width = 200;
	int height = 100;

	std::string fileName = "output.png";

	uint8_t* imageData = (uint8_t*)malloc(width * height * 3);

	int counter = 0;
	for (int i = height - 1; i >= 0; --i)
	{
		for (int j = 0; j < width; ++j)
		{
			imageData[counter++] = uint8_t(255.9 * float(j) / float(width));
			imageData[counter++] = uint8_t(255.9 * float(i) / float(height));
			imageData[counter++] = uint8_t(255.9 * 0.2f);
		}
	}

	std::cout << "Wrote file " << fileName << "\n";

	stbi_write_png(fileName.c_str(), width, height, 3, (void*)imageData, 0);

	free((void*)imageData);

	std::cout << "Press Enter to Continue";
	std::cin.ignore();
	return 0;
}