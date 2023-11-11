#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <vector>
using namespace std;

struct Header {

	char idLength;
	char colorMapType;
	char dataTypeCode;
	short colorMapOrigin;
	short colorMapLength;
	char colorMapDepth;
	short xOrigin;
	short yOrigin;
	short width;
	short height;
	char bitsPerPixel;
	char imageDescriptor;

};

struct Pixel {

	unsigned char blue;
	unsigned char green;
	unsigned char red;

};

struct Image {

	Header header;
	vector<Pixel> pixels;

	Image() {}
	void read(const string& inName) {

		ifstream file(inName, ios_base::binary);

		file.read((char*)&header.idLength, sizeof(header.idLength));
		file.read((char*)&header.colorMapType, sizeof(header.colorMapType));
		file.read((char*)&header.dataTypeCode, sizeof(header.dataTypeCode));
		file.read((char*)&header.colorMapOrigin, sizeof(header.colorMapOrigin));
		file.read((char*)&header.colorMapLength, sizeof(header.colorMapLength));
		file.read((char*)&header.colorMapDepth, sizeof(header.colorMapDepth));
		file.read((char*)&header.xOrigin, sizeof(header.xOrigin));
		file.read((char*)&header.yOrigin, sizeof(header.yOrigin));
		file.read((char*)&header.width, sizeof(header.width));
		file.read((char*)&header.height, sizeof(header.height));
		file.read((char*)&header.bitsPerPixel, sizeof(header.bitsPerPixel));
		file.read((char*)&header.imageDescriptor, sizeof(header.imageDescriptor));

		int imageSize = (int)(header.width * header.height);
		for (int i = 0; i < imageSize; i++) {
			Pixel pixel;

			file.read((char*)&pixel.blue, sizeof(pixel.blue));
			file.read((char*)&pixel.green, sizeof(pixel.green));
			file.read((char*)&pixel.red, sizeof(pixel.red));

			pixels.push_back(pixel);

		}

	}
	void write(const string& outName) {

		ofstream file(outName, ios_base::binary);

		file.write(&header.idLength, sizeof(header.idLength));
		file.write(&header.colorMapType, sizeof(header.colorMapType));
		file.write(&header.dataTypeCode, sizeof(header.dataTypeCode));
		file.write((char*)&header.colorMapOrigin, sizeof(header.colorMapOrigin));
		file.write((char*)&header.colorMapLength, sizeof(header.colorMapLength));
		file.write(&header.colorMapDepth, sizeof(header.colorMapDepth));
		file.write((char*)&header.xOrigin, sizeof(header.xOrigin));
		file.write((char*)&header.yOrigin, sizeof(header.yOrigin));
		file.write((char*)&header.width, sizeof(header.width));
		file.write((char*)&header.height, sizeof(header.height));
		file.write(&header.bitsPerPixel, sizeof(header.bitsPerPixel));
		file.write(&header.imageDescriptor, sizeof(header.imageDescriptor));

		for (int i = 0; i < pixels.size(); i++) {
			Pixel pixel = pixels[i];

			file.write((char*)&pixel.blue, sizeof(pixel.blue));
			file.write((char*)&pixel.green, sizeof(pixel.green));
			file.write((char*)&pixel.red, sizeof(pixel.red));

		}

	}
};

//helper methods

unsigned char sum(unsigned char& a, unsigned char& b) {
	int x = (int)a;
	int y = (int)b;
	int z = x + y;
	if (z > 255) {
		z = 255;
	}
	else if (z < 0) {
		z = 0;
	}
	return (unsigned char)z;
}

unsigned char subtract(unsigned char& a, unsigned char& b) {
	int x = (int)a;
	int y = (int)b;
	int z = x - y;
	if (z > 255) {
		z = 255;
	}
	else if (z < 0) {
		z = 0;
	}
	return (unsigned char)z;
}

unsigned char multiply(unsigned char& a, unsigned char& b) {
	float x = (float)a / 255;
	float y = (float)b / 255;
	float z = x * y;
	z *= 255;
	z += 0.5f;
	z = (int)z;
	return (unsigned char)z;
}

unsigned char screen(unsigned char& a, unsigned char& b) {
	float x = (float)a / 255;
	float y = (float)b / 255;

	x = 1 - x;
	y = 1 - y;

	float z = x * y;
	z = 1 - z;
	z *= 255;
	z += 0.5f;
	z = (int)z;
	return (unsigned char)z;
}

unsigned char overlay(unsigned char& a, unsigned char& b) {
	float x = (float)a / 255;
	float y = (float)b / 255;

	if (y <= 0.5) {
		float z = x * y;
		z *= (255 * 2);
		z += 0.5f;
		z = (int)z;
		return (unsigned char)z;
	}
	else {
		x = 1 - x;
		y = 1 - y;
		float z = x * y;
		z *= 2;
		z = 1 - z;
		z *= 255;
		z += 0.5f;
		z = (int)z;
		return (unsigned char)z;
	}
}

//task functions

void task1(Image& image1, Image& image2) {

	for (int i = 0; i < image1.pixels.size(); i++) {
		image1.pixels[i].blue = multiply(image1.pixels[i].blue, image2.pixels[i].blue);
		image1.pixels[i].green = multiply(image1.pixels[i].green, image2.pixels[i].green);
		image1.pixels[i].red = multiply(image1.pixels[i].red, image2.pixels[i].red);
	}

}

void task2(Image& image1, Image& image2) {

	for (int i = 0; i < image1.pixels.size(); i++) {
		image1.pixels[i].blue = subtract(image1.pixels[i].blue, image2.pixels[i].blue);
		image1.pixels[i].green = subtract(image1.pixels[i].green, image2.pixels[i].green);
		image1.pixels[i].red = subtract(image1.pixels[i].red, image2.pixels[i].red);
	}

}

void task3(Image& image1, Image& image2) {
	for (int i = 0; i < image1.pixels.size(); i++) {
		image1.pixels[i].blue = screen(image1.pixels[i].blue, image2.pixels[i].blue);
		image1.pixels[i].green = screen(image1.pixels[i].green, image2.pixels[i].green);
		image1.pixels[i].red = screen(image1.pixels[i].red, image2.pixels[i].red);
	}

}

Pixel* task4(string file1, string file2, string file3) {
	Image image1, image2, image3, image4;
	image1.read(file1);
	image2.read(file2);
	image3.read(file3);
	image4.read(file1);

	Pixel* multiplied_img = new Pixel[image1.pixels.size()];

	for (int i = 0; i < image1.pixels.size(); i++) {
		multiplied_img[i].blue = multiply(image1.pixels[i].blue, image2.pixels[i].blue);
		multiplied_img[i].green = multiply(image1.pixels[i].green, image2.pixels[i].green);
		multiplied_img[i].red = multiply(image1.pixels[i].red, image2.pixels[i].red);

		image4.pixels[i].blue = subtract(multiplied_img[i].blue, image3.pixels[i].blue);
		image4.pixels[i].green = subtract(multiplied_img[i].green, image3.pixels[i].green);
		image4.pixels[i].red = subtract(multiplied_img[i].red, image3.pixels[i].red);
	}

	image4.write("output/part4.tga");
	return multiplied_img;

}

void task5(Image& image1, Image& image2) {

	for (int i = 0; i < image1.pixels.size(); i++) {
		image1.pixels[i].blue = overlay(image1.pixels[i].blue, image2.pixels[i].blue);
		image1.pixels[i].green = overlay(image1.pixels[i].green, image2.pixels[i].green);
		image1.pixels[i].red = overlay(image1.pixels[i].red, image2.pixels[i].red);
	}

}

void task6(string file) {
	Image image1, image2;
	image1.read(file);
	image2.read(file);
	unsigned char g = 200;

	for (int i = 0; i < image1.pixels.size(); i++) {
		image2.pixels[i].green = sum(image1.pixels[i].green, g);
	}

	image2.write("output/part6.tga");

}

void addred(Image& image, int r) {
	int pix_r;
	for (int i = 0; i < image.pixels.size(); i++) {
		//image.pixels[i].red = sum(image.pixels[i].red, r);
		pix_r = (int)image.pixels[i].red;
		pix_r += r;
		if (pix_r > 255) {
			pix_r = 255;
		}
		if (pix_r < 0) {
			pix_r = 0;
		}
		image.pixels[i].red = (unsigned char)pix_r;
	}
}

void addgreen(Image& image, int g) {
	int pix_g;
	for (int i = 0; i < image.pixels.size(); i++) {
		pix_g = (int)image.pixels[i].green;
		pix_g += g;
		if (pix_g > 255) {
			pix_g = 255;
		}
		if (pix_g < 0) {
			pix_g = 0;
		}
		image.pixels[i].green = (unsigned char)pix_g;
	}
}

void addblue(Image& image, int b) {
	int pix_b;
	for (int i = 0; i < image.pixels.size(); i++) {
		pix_b = (int)image.pixels[i].blue;
		pix_b += b;
		if (pix_b > 255) {
			pix_b = 255;
		}
		if (pix_b < 0) {
			pix_b = 0;
		}
		image.pixels[i].blue = (unsigned char)pix_b;
	}
}

void task7(string file) {
	Image image1, image2;
	image1.read(file);
	image2.read(file);
	unsigned char b = 0;

	for (int i = 0; i < image1.pixels.size(); i++) {
		image2.pixels[i].blue = multiply(image1.pixels[i].blue, b);

		int clamp = (int)image1.pixels[i].red;
		clamp *= 4;
		if (clamp > 255) {
			clamp = 255;
		}

		image2.pixels[i].red = (unsigned char)clamp;
	}

	image2.write("output/part7.tga");

}

void task8(string file) {
	Image image1, image2;
	image1.read(file);
	image2.read(file);

	for (int i = 0; i < image1.pixels.size(); i++) {
		// blue channel
		image2.pixels[i].blue = image1.pixels[i].blue;
		image2.pixels[i].green = image1.pixels[i].blue;
		image2.pixels[i].red = image1.pixels[i].blue;
	}
	image2.write("output/part8_b.tga");

	for (int i = 0; i < image1.pixels.size(); i++) {
		// green channel
		image2.pixels[i].blue = image1.pixels[i].green;
		image2.pixels[i].green = image1.pixels[i].green;
		image2.pixels[i].red = image1.pixels[i].green;
	}
	image2.write("output/part8_g.tga");

	for (int i = 0; i < image1.pixels.size(); i++) {
		// red channel
		image2.pixels[i].blue = image1.pixels[i].red;
		image2.pixels[i].green = image1.pixels[i].red;
		image2.pixels[i].red = image1.pixels[i].red;
	}
	image2.write("output/part8_r.tga");
}

void onlyred(Image& image) {
	for (int i = 0; i < image.pixels.size(); i++) {
		image.pixels[i].blue = image.pixels[i].red;
		image.pixels[i].green = image.pixels[i].red;
	}
}

void onlygreen(Image& image) {
	for (int i = 0; i < image.pixels.size(); i++) {
		image.pixels[i].red = image.pixels[i].green;
		image.pixels[i].blue = image.pixels[i].green;
	}
}

void onlyblue(Image& image) {
	for (int i = 0; i < image.pixels.size(); i++) {
		image.pixels[i].red = image.pixels[i].blue;
		image.pixels[i].green = image.pixels[i].blue;
	}
}

void scalered(Image& image, int scale) {
	int clamp;
	for (int i = 0; i < image.pixels.size(); i++) {
		clamp = (int)image.pixels[i].red;
		clamp *= scale;
		if (clamp > 255) {
			clamp = 255;
		}

		image.pixels[i].red = (unsigned char)clamp;
	}
}

void scalegreen(Image& image, int scale) {
	int clamp;
	for (int i = 0; i < image.pixels.size(); i++) {
		clamp = (int)image.pixels[i].green;
		clamp *= scale;
		if (clamp > 255) {
			clamp = 255;
		}

		image.pixels[i].green = (unsigned char)clamp;
	}
}

void scaleblue(Image& image, int scale) {
	int clamp;
	for (int i = 0; i < image.pixels.size(); i++) {
		clamp = (int)image.pixels[i].blue;
		clamp *= scale;
		if (clamp > 255) {
			clamp = 255;
		}

		image.pixels[i].blue = (unsigned char)clamp;
	}
}

void task9(Image& image1, Image& image2, Image& image3) {

	for (int i = 0; i < image1.pixels.size(); i++) {
		image3.pixels[i].blue = image1.pixels[i].blue;
		image3.pixels[i].green = image2.pixels[i].green;
	}

}

void flip(Image& image) {

	Pixel* clone = new Pixel[image.pixels.size()];

	for (int i = 0; i < image.pixels.size(); i++) {
		clone[i].blue = image.pixels[i].blue;
		clone[i].green = image.pixels[i].green;
		clone[i].red = image.pixels[i].red;
	}

	for (int i = 0; i < image.pixels.size(); i++) {
		image.pixels[i].blue = clone[image.pixels.size() - 1 - i].blue;
		image.pixels[i].green = clone[image.pixels.size() - 1 - i].green;
		image.pixels[i].red = clone[image.pixels.size() - 1 - i].red;
	}

}

// old main
	/*task1("input/layer1.tga", "input/pattern1.tga");

	task2("input/car.tga", "input/layer2.tga");

	Pixel* task3_arr = task3("input/layer1.tga", "input/pattern2.tga", "input/text.tga");

	Pixel* task4_arr = task4("input/layer2.tga", "input/circles.tga", "input/pattern2.tga");

	task5("input/layer1.tga", "input/pattern1.tga");

	task6("input/car.tga");

	task7("input/car.tga");

	task8("input/car.tga");

	task9("input/layer_blue.tga", "input/layer_green.tga", "input/layer_red.tga");

	task10("input/text2.tga");*/

int main(int argc, char* argv[]) {

	fstream image2_file, image3_file, image_mult_file, image_sub_file, image_overlay_file, image_screen_file;
	Image tracking, image2, image3, image_mult, image_sub, image_overlay, image_screen;
	string current, output, file2, file3, file_mult, file_sub, file_overlay, file_screen;
	unsigned char modifier;
	int scale, to_add;

	if (argc == 1 || strcmp(argv[1], "--help") == 0) {
		cout << "Project 2: Image Processing, Fall 2023\n\nUsage:\n\t./project2.out [output] [firstImage] [method] [...]";
		return 0;
	}
	if (argc >= 2) {
		output = argv[1];
		if (output.size() <= 4 || output.substr(output.size() - 4, 4) != ".tga") {
			cout << "Invalid file name.";
			return 0;
		}
	}
	if (argc >= 3) {

		string tracking_str = argv[2];
		fstream tracking_file;

		tracking_file.open(tracking_str);

		if (tracking_str.size() <= 4 || tracking_str.substr(tracking_str.size() - 4, 4) != ".tga") {
			cout << "Invalid file name.";
			return 0;
		}

		if (tracking_file.is_open()) {
			tracking.read(tracking_str);
		}
		else {
			cout << "File does not exist.";
			return 0;
		}

	}

	if (argc == 3) {
		cout << "Invalid method name.";
		return 0;
	}
	try {
		int argI = 3;
		while (argI < argc) {
			if (strcmp(argv[argI], "multiply") == 0) {
				argI++;
				if (argI >= argc) {
					cout << "Missing argument.";
					return 0;
				}
				current = argv[argI];
				if (current.size() <= 4 || current.substr(current.size() - 4, 4) != ".tga") {
					cout << "Invalid argument, invalid file name.";
					return 0;
				}

				image_mult_file.open(current);

				if (image_mult_file.is_open()) {
					image_mult.read(current);
				}
				else {
					cout << "Invalid argument, file does not exist.";
					return 0;
				}

				task1(tracking, image_mult);

				image_mult_file.close();

			}

			else if (strcmp(argv[argI], "subtract") == 0) {
				argI++;
				if (argI >= argc) {
					cout << "Missing argument.";
					return 0;
				}

				current = argv[argI];
				if (current.size() <= 4 || current.substr(current.size() - 4, 4) != ".tga") {
					cout << "Invalid argument, invalid file name.";
					return 0;
				}

				image_sub_file.open(current);

				if (image_sub_file.is_open()) {
					image_sub.read(current);
				}
				else {
					cout << "Invalid argument, file does not exist.";
					return 0;
				}

				task2(tracking, image_sub);

				image_sub_file.close();

			}

			else if (strcmp(argv[argI], "overlay") == 0) {
				argI++;
				if (argI >= argc) {
					cout << "Missing argument.";
					return 0;
				}

				current = argv[argI];
				if (current.size() <= 4 || current.substr(current.size() - 4, 4) != ".tga") {
					cout << "Invalid argument, invalid file name.";
					return 0;
				}

				image_overlay_file.open(current);

				if (image_overlay_file.is_open()) {
					image_overlay.read(current);
				}
				else {
					cout << "Invalid argument, file does not exist.";
					return 0;
				}

				task5(tracking, image_overlay);

				image_overlay_file.close();

			}

			else if (strcmp(argv[argI], "screen") == 0) {
				argI++;
				if (argI >= argc) {
					cout << "Missing argument.";
					return 0;
				}

				current = argv[argI];
				if (current.size() <= 4 || current.substr(current.size() - 4, 4) != ".tga") {
					cout << "Invalid argument, invalid file name.";
					return 0;
				}

				image_screen_file.open(current);

				if (image_screen_file.is_open()) {
					image_screen.read(current);
				}
				else {
					cout << "Invalid argument, file does not exist.";
					return 0;
				}

				task3(tracking, image_screen);

				image_screen_file.close();

			}

			else if (strcmp(argv[argI], "combine") == 0) {
				argI += 2;
				if (argI >= argc) {
					cout << "Missing argument.";
					return 0;
				}

				current = argv[argI - 1];
				if (current.size() <= 4 || current.substr(current.size() - 4, 4) != ".tga") {
					cout << "Invalid argument, invalid file name.";
					return 0;
				}

				image2_file.open(current);

				if (image2_file.is_open()) {
					image2.read(current);
				}
				else {
					cout << "Invalid argument, file does not exist.";
					return 0;
				}

				current = argv[argI];
				if (current.size() <= 4 || current.substr(current.size() - 4, 4) != ".tga") {
					cout << "Invalid argument, invalid file name.";
					return 0;
				}

				image3_file.open(current);

				if (image3_file.is_open()) {
					image3.read(current);
				}
				else {
					cout << "Invalid argument, file does not exist.";
					return 0;
				}

				/*file2 = argv[argI - 1];
				file3 = argv[argI];
				image2.read(file2);
				image3.read(file3);*/

				//put in BGR order
				task9(image3, image2, tracking);

				image2_file.close();
				image3_file.close();

			}

			else if (strcmp(argv[argI], "flip") == 0) {
				flip(tracking);
			}

			else if (strcmp(argv[argI], "onlyred") == 0) {
				onlyred(tracking);
			}

			else if (strcmp(argv[argI], "onlygreen") == 0) {
				onlygreen(tracking);
			}

			else if (strcmp(argv[argI], "onlyblue") == 0) {
				onlyblue(tracking);
			}

			else if (strcmp(argv[argI], "addred") == 0) {
				argI++;
				if (argI >= argc) {
					cout << "Missing argument.";
					return 0;
				}
				try {
					//modifier = (unsigned char)stoi((string)argv[argI]);
					to_add = stoi((string)argv[argI]);
					addred(tracking, to_add);
				}
				catch (exception& e) {
					cout << "Invalid argument, expected number.";
					return 0;
				}
			}

			else if (strcmp(argv[argI], "addgreen") == 0) {
				argI++;
				if (argI >= argc) {
					cout << "Missing argument.";
					return 0;
				}
				try {
					to_add = stoi((string)argv[argI]);
					addgreen(tracking, to_add);
				}
				catch (exception& e) {
					cout << "Invalid argument, expected number.";
					return 0;
				}

			}

			else if (strcmp(argv[argI], "addblue") == 0) {
				argI++;
				if (argI >= argc) {
					cout << "Missing argument.";
					return 0;
				}
				try {
					to_add = stoi((string)argv[argI]);
					addblue(tracking, to_add);
				}
				catch (exception& e) {
					cout << "Invalid argument, expected number.";
					return 0;
				}

			}

			else if (strcmp(argv[argI], "scalered") == 0) {
				argI++;
				if (argI >= argc) {
					cout << "Missing argument.";
					return 0;
				}
				try {
					scale = stoi((string)argv[argI]);
					scalered(tracking, scale);
				}
				catch (exception& e) {
					cout << "Invalid argument, expected number.";
					return 0;
				}

			}

			else if (strcmp(argv[argI], "scalegreen") == 0) {
				argI++;
				if (argI >= argc) {
					cout << "Missing argument.";
					return 0;
				}
				try {
					scale = stoi((string)argv[argI]);
					scalegreen(tracking, scale);
				}
				catch (exception& e) {
					cout << "Invalid argument, expected number.";
					return 0;
				}

			}

			else if (strcmp(argv[argI], "scaleblue") == 0) {
				argI++;
				if (argI >= argc) {
					cout << "Missing argument.";
					return 0;
				}
				try {
					scale = stoi((string)argv[argI]);
					scaleblue(tracking, scale);
				}
				catch (exception& e) {
					cout << "Invalid argument, expected number.";
					return 0;
				}

			}

			else {
				cout << "Invalid method name.";
				return 0;
			}

			argI++;
		}
	}
	catch (invalid_argument& e) {
		cout << "caught";
	}

	tracking.write(output);
	return 0;
}