#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<fstream>
#include<string>
#include<Windows.h>
#include<vector>
#include<assert.h>
using namespace std;

void stob(int* gb, int tar) { //�������� ��ȯ
	gb[0] = 0;
	int initbi = 64;
	for (int i = 1; i <= 7; i++) {
		gb[i] = tar / initbi;
		tar %= initbi;
		initbi /= 2;
	}
}
int btos(int* gb) { //�ƽ�Ű �ڵ� �����η� ��ȯ
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

	if (tmp == 'e') { //������������ e(���ڵ�)�� ���� ���

		BITMAPFILEHEADER    bf;

		BITMAPINFOHEADER    bi;


		ifstream fin;

		fin.open("origin.bmp", ios_base::in | ios_base::binary); //origin.bmp������ ����Ʈ�� �о� ���Դϴ�

		fin.read((char*)&bf, sizeof(BITMAPFILEHEADER)); // ������

		FILE* fp = fopen("origin.bmp", "rb");


		fread(&bf, sizeof(unsigned char), sizeof(BITMAPFILEHEADER), fp);// ����� ������ 14����Ʈ ����
		fread(&bi, sizeof(unsigned char), sizeof(BITMAPINFOHEADER), fp);// ������ ���� 40����Ʈ ����
		unsigned char* data = (unsigned char*)malloc(bi.biSizeImage); // �̹����� ������ ��ŭ �Ҵ�

		fread(data, sizeof(unsigned char), bi.biSizeImage, fp); // �̹��� �ȼ� �����͸� data ������ ����

		fclose(fp); //data�� ��Ҵ�.

		string inputs; //�Է¹��ڿ�
		getline(cin, inputs); // ���� ���� �Է�

		int index = 0; // data ���� ���� ������ó�� �۵� �մϴ�
		for (int i = 0; i < inputs.length(); i++) { //���� �ϳ��� ���ڵ� 
			int getbinary[8]; // �ϳ��� ���ڸ� �������� �ٲپ� �ݴϴ�. index 0 ( �������θ� ǥ�� )�� �ϴ� 0���� �ʱ�ȭ
			stob(getbinary, (int)inputs[i]); // ex) z (�ƽ�Ű �ڵ� 122 ) --> 01111010 (�� �պ�Ʈ�� ���� ���� ǥ��)

			if (data[index] % 2 == 0) { //�����Ʈ�� Ȧ���� �ٲپ� ���ݺ��� 7���� ����Ʈ�� ������ ���������� ǥ�� 
				data[index]++;
			}
			for (int j = 1; j <= 7; j++) { //�������� �ٲ� ���ڸ� �����ݴϴ� 1�� ����� Ȧ����, 0�� ����� ¦����
				if (getbinary[j] == 1) {
					if (data[j + index] % 2 == 0) { //Ȧ���� 1
						data[j + index]++;
					}
				}
				else { //¦���� 0
					if (data[j + index] % 2 == 1) {
						if (data[j + index] == 255) data[j + index]--;
						else data[j + index]++;
					}
				}
			}
			index += 8; //������ �̵�
		}
		if (data[index] % 2 == 1) { //������ ��� ���� ��, �� ���� ��Ʈ�� ¦���� ����� ���ķδ� ������ ������ ǥ�� 
			if (data[index] == 255) data[index]--;
			else data[index]++;
		}
		//��º�
		ofstream fout;
		fout.open("stego.bmp", ios::binary);
		assert(fout.is_open());
		fout.write((LPSTR)&bf, sizeof(BITMAPFILEHEADER)); //14����Ʈ
		fout.write((LPSTR)&bi, sizeof(BITMAPINFOHEADER)); //40����Ʈ
		fout.write((LPSTR)data, bi.biSizeImage);

		fout.close();
		system("pause");

	}
	if (tmp == 'd') { //���ڵ�
		BITMAPFILEHEADER    bf;

		BITMAPINFOHEADER    bi;


		ifstream fin;

		fin.open("stego.bmp", ios_base::in | ios_base::binary); //origin.bmp������ ����Ʈ�� �о� ���Դϴ�

		fin.read((char*)&bf, sizeof(BITMAPFILEHEADER)); // ������

		FILE* fp = fopen("stego.bmp", "rb");


		fread(&bf, sizeof(unsigned char), sizeof(BITMAPFILEHEADER), fp);// ����� ������ 14����Ʈ
		fread(&bi, sizeof(unsigned char), sizeof(BITMAPINFOHEADER), fp);// ������ ���� 40����Ʈ ����
		unsigned char* data = (unsigned char*)malloc(bi.biSizeImage); //�̹��� ������ ��ŭ �Ҵ�

		int endpoint = 527516; //������
		fread(data, sizeof(unsigned char), bi.biSizeImage, fp); // �̹��� �ȼ� �����͸� data ������ ����

		fclose(fp); //data�� ��Ҵ�.

		
		int index = 0; //������ ����
		while (1) {
			if (data[index] % 2 == 0) break; //8���� ��Ʈ �� �� �պ�Ʈ�� Ȧ���̸� ������ ������ �ǹ�
			int getbinary[8];
			getbinary[0] = 0;
			for (int a = 1; a <= 7; a++) { // ������ �̾ƿ���
				if (data[a + index] % 2 == 1) getbinary[a] = 1;
				else getbinary[a] = 0;
			}
			cout << (char)btos(getbinary);
			index += 8; //������ �̵�
		}
		cout << "\n";
		system("pause");
	}


	return 0;
}