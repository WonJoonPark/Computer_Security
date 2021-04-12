#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<fstream>
#include<string>
#include<Windows.h>
#include<vector>
#include<assert.h>
using namespace std;

void stob(int* gb, int tar) { //이진수로 변환
	gb[0] = 0;
	int initbi = 64;
	for (int i = 1; i <= 7; i++) {
		gb[i] = tar / initbi;
		tar %= initbi;
		initbi /= 2;
	}
}
int btos(int* gb) { //아스키 코드 값으로로 변환
	int rval = 0;
	int initbi = 64;
	for (int i = 1; i <= 7; i++) {
		if (gb[i] == 1) rval += (initbi);
		initbi /= 2;
	}
	return rval;
}
//int argc, char* argv[]
int main(int argc, char* argv[]) {
	char tmp=*argv[1];

	if (tmp == 'e') { //실행조건으로 e(인코딩)이 들어올 경우

		BITMAPFILEHEADER    bf;

		BITMAPINFOHEADER    bi;


		ifstream fin;

		fin.open("origin.bmp", ios_base::in | ios_base::binary); //origin.bmp파일을 바이트로 읽어 들입니다

		fin.read((char*)&bf, sizeof(BITMAPFILEHEADER)); // 사이즈

		FILE* fp = fopen("origin.bmp", "rb");


		fread(&bf, sizeof(unsigned char), sizeof(BITMAPFILEHEADER), fp);// 헤더에 사이즈 14바이트 저장
		fread(&bi, sizeof(unsigned char), sizeof(BITMAPINFOHEADER), fp);// 나머지 정보 40바이트 저장
		unsigned char* data = (unsigned char*)malloc(bi.biSizeImage); // 이미지의 사이즈 만큼 할당

		fread(data, sizeof(unsigned char), bi.biSizeImage, fp); // 이미지 픽셀 데이터를 data 변수에 저장

		fclose(fp); //data에 담았다.

		string inputs; //입력문자열
		getline(cin, inputs); // 공백 포함 입력

		int index = 0; // data 변수 에서 포인터처럼 작동 합니다
		for (int i = 0; i < inputs.length(); i++) { //문자 하나씩 인코딩 
			int getbinary[8]; // 하나의 문자를 이진수로 바꾸어 줍니다. index 0 ( 변동여부를 표시 )는 일단 0으로 초기화
			stob(getbinary, (int)inputs[i]); // ex) z (아스키 코드 122 ) --> 01111010 (맨 앞비트는 변동 여부 표시)

			if (data[index] % 2 == 0) { //헤더비트를 홀수로 바꾸어 지금부터 7개의 바이트는 정보가 숨어있음을 표시 
				data[index]++;
			}
			for (int j = 1; j <= 7; j++) { //이진수로 바꾼 문자를 숨겨줍니다 1을 숨길시 홀수로, 0을 숨길시 짝수로
				if (getbinary[j] == 1) {
					if (data[j + index] % 2 == 0) { //홀수는 1
						data[j + index]++;
					}
				}
				else { //짝수는 0
					if (data[j + index] % 2 == 1) {
						if (data[j + index] == 255) data[j + index]--;
						else data[j + index]++;
					}
				}
			}
			index += 8; //포인터 이동
		}
		if (data[index] % 2 == 1) { //정보를 모두 숨긴 후, 그 다음 비트를 짝수로 만들어 이후로는 정보가 끝임을 표시 
			if (data[index] == 255) data[index]--;
			else data[index]++;
		}
		//출력부
		ofstream fout;
		fout.open("stego.bmp", ios::binary);
		assert(fout.is_open());
		fout.write((LPSTR)&bf, sizeof(BITMAPFILEHEADER)); //14바이트
		fout.write((LPSTR)&bi, sizeof(BITMAPINFOHEADER)); //40바이트
		fout.write((LPSTR)data, bi.biSizeImage);

		fout.close();
		system("pause");

	}
	if (tmp == 'd') { //디코딩
		BITMAPFILEHEADER    bf;

		BITMAPINFOHEADER    bi;


		ifstream fin;

		fin.open("stego.bmp", ios_base::in | ios_base::binary); //origin.bmp파일을 바이트로 읽어 들입니다

		fin.read((char*)&bf, sizeof(BITMAPFILEHEADER)); // 사이즈

		FILE* fp = fopen("stego.bmp", "rb");


		fread(&bf, sizeof(unsigned char), sizeof(BITMAPFILEHEADER), fp);// 헤더에 사이즈 14바이트
		fread(&bi, sizeof(unsigned char), sizeof(BITMAPINFOHEADER), fp);// 나머지 정보 40바이트 저장
		unsigned char* data = (unsigned char*)malloc(bi.biSizeImage); //이미지 사이즈 만큼 할당

		int endpoint = 527516; //마지막
		fread(data, sizeof(unsigned char), bi.biSizeImage, fp); // 이미지 픽셀 데이터를 data 변수에 저장

		fclose(fp); //data에 담았다.

		
		int index = 0; //포인터 역할
		while (1) {
			if (data[index] % 2 == 0) break; //8개의 비트 중 맨 앞비트가 홀수이면 정보가 없음을 의미
			int getbinary[8];
			getbinary[0] = 0;
			for (int a = 1; a <= 7; a++) { // 데이터 뽑아오기
				if (data[a + index] % 2 == 1) getbinary[a] = 1;
				else getbinary[a] = 0;
			}
			cout << (char)btos(getbinary);
			index += 8; //포인터 이동
		}
		cout << "\n";
		system("pause");
	}


	return 0;
}