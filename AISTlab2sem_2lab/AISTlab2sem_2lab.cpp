#define STB_IMAGE_IMPLEMENTATION
#define _USE_MATH_DEFINES
#include <fstream>
#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <unordered_map>
#include <bitset>
#define LENNA_RAW "C:\\Users\\aasto\\source\\repos\\AISTlab2sem_2lab\\rawf\\Lenna.raw"
#define Y_MODE false
#define CR_CB_MODE true
#define QUANT_QUALITY 20
#define RGB_MODE false
#define BW_MODE true

using namespace std;

vector<vector<short int>> zigzagIndexes = { {0, 0},{0, 1},{1, 0},{2, 0},{1, 1},{0, 2},{0, 3},{1, 2},{2, 1},{3, 0},{4, 0},{3, 1},{2, 2},{1, 3},{0, 4},{0, 5},{1, 4},{2, 3},{3, 2},{4, 1},{5, 0},{6, 0},{5, 1},{4, 2},{3, 3},{2, 4},{1, 5},{0, 6},{0, 7},{1, 6},{2, 5},{3, 4},{4, 3},{5, 2},{6, 1},{7, 0},{7,1},{6,2},{5,3},{4,4},{3,5},{2,6},{1,7},{2,7},{3,6},{4,5},{5,4},{6,3},{7,2},{7,3},{6,4},{5,5},{4,6},{3,7},{4,7},{5,6},{6,5},{7,4},{7,5},{6,6},{5,7},{6,7},{7,6},{7,7} };
vector<vector<float>> quantingTableDC = { { 16.0,11.0,10.0,16.0,24.0,40.0,51.0,61.0 }, { 12.0,12.0,14.0,19.0,26.0,58.0,60.0,55.0 }, { 14.0,13.0,16.0,24.0,40.0,57.0,69.0,56.0 }, { 14.0,17.0,22.0,29.0,51.0,87.0,80.0,62.0 }, { 18.0,22.0,37.0,56.0,68.0,109.0,103.0,77.0 }, { 24.0,35.0,55.0,64.0,81.0,104.0,113.0,92.0 }, { 49.0,64.0,78.0,87.0,103.0,121.0,120.0,101.0 }, { 72.0,92.0,95.0,98.0,112.0,100.0,103.0,99.0 } };
vector<vector<float>> quantingTableAC = { { 17.0,18.0,24.0,47.0,99.0,99.0,99.0,99.0 }, { 18.0,21.0,26.0,66.0,99.0,99.0,99.0,99.0 }, { 24.0,26.0,56.0,99.0,99.0,99.0,99.0,99.0 }, { 47.0,66.0,99.0,99.0,99.0,99.0,99.0,99.0 }, { 99.0,99.0,99.0,99.0,99.0,99.0,99.0,99.0 }, { 99.0,99.0,99.0,99.0,99.0,99.0,99.0,99.0 }, { 99.0,99.0,99.0,99.0,99.0,99.0,99.0,99.0 }, { 99.0,99.0,99.0,99.0,99.0,99.0,99.0,99.0 } };

vector<vector<string>> luminanceDCstr = { {"0","000"},{"11","010"},{"12","011"},{"13","100"},{"14","101"},{"15","110"},{"16","1110"},{"17","11110"},{"18","111110"},{"19","1111110"},{"10","11111110"},{"11","111111110"}, };
vector<vector<short int>> luminanceDCint = { {3},{3},{3},{3},{3},{3},{4},{5},{6},{7},{8},{9}, };
vector<vector<string>> chrominanceDCstr = { {"0","000"},{"11","01"},{"12","10"},{"13","110"},{"14","1110"},{"15","11110"},{"16","111110"},{"17","1111110"},{"18","11111110"},{"19","111111110"},{"10","1111111110"},{"11","11111111110"}, };
vector<vector<short int>> chrominanceDCint = { {3},{2},{2},{3},{4},{5},{6},{7},{8},{9},{10},{11}, };
vector<vector<string>> luminanceACstr = { {"00","1010"},{"01","00"},{"02","01"},{"03","100"},{"04","1011"},{"05","11010"},{"06","1111000"},{"07","11111000"},{"08","1111110110"},{"09","1111111110000010"},{"0A","1111111110000011"},{"11","1100"},{"12","11011"},{"13","1111001"},{"14","111110110"},{"15","11111110110"},{"16","1111111110000100"},{"17","1111111110000101"},{"18","1111111110000110"},{"19","1111111110000111"},{"1A","1111111110001000"},{"21","11100"},{"22","11111001"},{"23","1111110111"},{"24","111111110100"},{"25","1111111110001001"},{"26","1111111110001010"},{"27","1111111110001011"},{"28","1111111110001100"},{"29","1111111110001101"},{"2A","1111111110001110"},{"31","111010"},{"32","111110111"},{"33","111111110101"},{"34","1111111110001111"},{"35","1111111110010000"},{"36","1111111110010001"},{"37","1111111110010010"},{"38","1111111110010011"},{"39","1111111110010100"},{"3A","1111111110010101"},{"41","111011"},{"42","1111111000"},{"43","1111111110010110"},{"44","1111111110010111"},{"45","1111111110011000"},{"46","1111111110011001"},{"47","1111111110011010"},{"48","1111111110011011"},{"49","1111111110011100"},{"4A","1111111110011101"},{"51","1111010"},{"52","11111110111"},{"53","1111111110011110"},{"54","1111111110011111"},{"55","1111111110100000"},{"56","1111111110100001"},{"57","1111111110100010"},{"58","1111111110100011"},{"59","1111111110100100"},{"5A","1111111110100101"},{"61","1111011"},{"62","111111110110"},{"63","1111111110100110"},{"64","1111111110100111"},{"65","1111111110101000"},{"66","1111111110101001"},{"67","1111111110101010"},{"68","1111111110101011"},{"69","1111111110101100"},{"6A","1111111110101101"},{"71","11111010"},{"72","111111110111"},{"73","1111111110101110"},{"74","1111111110101111"},{"75","1111111110110000"},{"76","1111111110110001"},{"77","1111111110110010"},{"78","1111111110110011"},{"79","1111111110110100"},{"7A","1111111110110101"},{"81","111111000"},{"82","111111111000000"},{"83","1111111110110110"},{"84","1111111110110111"},{"85","1111111110111000"},{"86","1111111110111001"},{"87","1111111110111010"},{"88","1111111110111011"},{"89","1111111110111100"},{"8A","1111111110111101"},{"91","111111001"},{"92","1111111110111110"},{"93","1111111110111111"},{"94","1111111111000000"},{"95","1111111111000001"},{"96","1111111111000010"},{"97","1111111111000011"},{"98","1111111111000100"},{"99","1111111111000101"},{"9A","1111111111000110"},{"A1","111111010"},{"A2","1111111111000111"},{"A3","1111111111001000"},{"A4","1111111111001001"},{"A5","1111111111001010"},{"A6","1111111111001011"},{"A7","1111111111001100"},{"A8","1111111111001101"},{"A9","1111111111001110"},{"AA","1111111111001111"},{"B1","1111111001"},{"B2","1111111111010000"},{"B3","1111111111010001"},{"B4","1111111111010010"},{"B5","1111111111010011"},{"B6","1111111111010100"},{"B7","1111111111010101"},{"B8","1111111111010110"},{"B9","1111111111010111"},{"BA","1111111111011000"},{"C1","1111111010"},{"C2","1111111111011001"},{"C3","1111111111011010"},{"C4","1111111111011011"},{"C5","1111111111011100"},{"C6","1111111111011101"},{"C7","1111111111011110"},{"C8","1111111111011111"},{"C9","1111111111100000"},{"CA","1111111111100001"},{"D1","11111111000"},{"D2","1111111111100010"},{"D3","1111111111100011"},{"D4","1111111111100100"},{"D5","1111111111100101"},{"D6","1111111111100110"},{"D7","1111111111100111"},{"D8","1111111111101000"},{"D9","1111111111101001"},{"DA","1111111111101010"},{"E1","1111111111101011"},{"E2","1111111111101100"},{"E3","1111111111101101"},{"E4","1111111111101110"},{"E5","1111111111101111"},{"E6","1111111111110000"},{"E7","1111111111110001"},{"E8","1111111111110010"},{"E9","1111111111110011"},{"EA","1111111111110100"},{"F0","11111111001"},{"F1","1111111111110101"},{"F2","1111111111110110"},{"F3","1111111111110111"},{"F4","1111111111111000"},{"F5","1111111111111001"},{"F6","1111111111111010"},{"F7","1111111111111011"},{"F8","1111111111111100"},{"F9","1111111111111101"},{"FA","1111111111111110"}, };
vector<vector<short int>> luminanceACint = { {4},{2},{2},{3},{4},{5},{7},{8},{10},{16},{16},{4},{5},{7},{9},{11},{16},{16},{16},{16},{16},{5},{8},{10},{12},{16},{16},{16},{16},{16},{16},{6},{9},{12},{16},{16},{16},{16},{16},{16},{16},{6},{10},{16},{16},{16},{16},{16},{16},{16},{16},{7},{11},{16},{16},{16},{16},{16},{16},{16},{16},{7},{12},{16},{16},{16},{16},{16},{16},{16},{16},{8},{12},{16},{16},{16},{16},{16},{16},{16},{16},{9},{15},{16},{16},{16},{16},{16},{16},{16},{16},{9},{16},{16},{16},{16},{16},{16},{16},{16},{16},{9},{16},{16},{16},{16},{16},{16},{16},{16},{16},{10},{16},{16},{16},{16},{16},{16},{16},{16},{16},{10},{16},{16},{16},{16},{16},{16},{16},{16},{16},{11},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16},{11},{16},{16},{16},{16},{16},{16},{16},{16},{16},{16}, };
vector<vector<string>> chrominanceACstr = { {"00","00"},{"01","01"},{"02","100"},{"03","1010"},{"04","11000"},{"05","11001"},{"06","111000"},{"07","1111000"},{"08","111110100"},{"09","1111110110"},{"0A","111111110100"},{"11","1011"},{"12","111001"},{"13","11110110"},{"14","111110101"},{"15","11111110110"},{"16","111111110101"},{"17","1111111110001000"},{"18","1111111110001001"},{"19","1111111110001010"},{"1A","1111111110001011"},{"21","11010"},{"22","11110111"},{"23","1111110111"},{"24","111111110110"},{"25","111111111000010"},{"26","1111111110001100"},{"27","1111111110001101"},{"28","1111111110001110"},{"29","1111111110001111"},{"2A","1111111110010000"},{"31","11011"},{"32","11111000"},{"33","1111111000"},{"34","111111110111"},{"35","1111111110010001"},{"36","1111111110010010"},{"37","1111111110010011"},{"38","1111111110010100"},{"39","1111111110010101"},{"3A","1111111110010110"},{"41","111010"},{"42","111110110"},{"43","1111111110010111"},{"44","1111111110011000"},{"45","1111111110011001"},{"46","1111111110011010"},{"47","1111111110011011"},{"48","1111111110011100"},{"49","1111111110011101"},{"4A","1111111110011110"},{"51","111011"},{"52","1111111001"},{"53","1111111110011111"},{"54","1111111110100000"},{"55","1111111110100001"},{"56","1111111110100010"},{"57","1111111110100011"},{"58","1111111110100100"},{"59","1111111110100101"},{"5A","1111111110100110"},{"61","1111001"},{"62","11111110111"},{"63","1111111110100111"},{"64","1111111110101000"},{"65","1111111110101001"},{"66","1111111110101010"},{"67","1111111110101011"},{"68","1111111110101100"},{"69","1111111110101101"},{"6A","1111111110101110"},{"71","1111010"},{"72","11111111000"},{"73","1111111110101111"},{"74","1111111110110000"},{"75","1111111110110001"},{"76","1111111110110010"},{"77","1111111110110011"},{"78","1111111110110100"},{"79","1111111110110101"},{"7A","1111111110110110"},{"81","11111001"},{"82","1111111110110111"},{"83","1111111110111000"},{"84","1111111110111001"},{"85","1111111110111010"},{"86","1111111110111011"},{"87","1111111110111100"},{"88","1111111110111101"},{"89","1111111110111110"},{"8A","1111111110111111"},{"91","111110111"},{"92","1111111111000000"},{"93","1111111111000001"},{"94","1111111111000010"},{"95","1111111111000011"},{"96","1111111111000100"},{"97","1111111111000101"},{"98","1111111111000110"},{"99","1111111111000111"},{"9A","1111111111001000"},{"A1","111111000"},{"A2","1111111111001001"},{"A3","1111111111001010"},{"A4","1111111111001011"},{"A5","1111111111001100"},{"A6","1111111111001101"},{"A7","1111111111001110"},{"A8","1111111111001111"},{"A9","1111111111010000"},{"AA","1111111111010001"},{"B1","111111001"},{"B2","1111111111010010"},{"B3","1111111111010011"},{"B4","1111111111010100"},{"B5","1111111111010101"},{"B6","1111111111010110"},{"B7","1111111111010111"},{"B8","1111111111011000"},{"B9","1111111111011001"},{"BA","1111111111011010"},{"C1","111111010"},{"C2","1111111111011011"},{"C3","1111111111011100"},{"C4","1111111111011101"},{"C5","1111111111011110"},{"C6","1111111111011111"},{"C7","1111111111100000"},{"C8","1111111111100001"},{"C9","1111111111100010"},{"CA","1111111111100011"},{"D1","11111111001"},{"D2","1111111111100100"},{"D3","1111111111100101"},{"D4","1111111111100110"},{"D5","1111111111100111"},{"D6","1111111111101000"},{"D7","1111111111101001"},{"D8","1111111111101010"},{"D9","1111111111101011"},{"DA","1111111111101100"},{"E1","11111111100000"},{"E2","1111111111101101"},{"E3","1111111111101110"},{"E4","1111111111101111"},{"E5","1111111111110000"},{"E6","1111111111110001"},{"E7","1111111111110010"},{"E8","1111111111110011"},{"E9","1111111111110100"},{"EA","1111111111110101"},{"F0","1111111010"},{"F1","111111111000011"},{"F2","1111111111110110"},{"F3","1111111111110111"},{"F4","1111111111111000"},{"F5","1111111111111001"},{"F6","1111111111111010"},{"F7","1111111111111011"},{"F8","1111111111111100"},{"F9","1111111111111101"},{"FA","1111111111111110"}, };
vector<vector<short int>> chrominanceACint = { {2},{2},{3},{4},{5},{5},{6},{7},{9},{10},{12},{4},{6},{8},{9},{11},{12},{16},{16},{16},{16},{5},{8},{10},{12},{15},{16},{16},{16},{16},{16},{5},{8},{10},{12},{16},{16},{16},{16},{16},{16},{6},{9},{16},{16},{16},{16},{16},{16},{16},{16},{6},{10},{16},{16},{16},{16},{16},{16},{16},{16},{7},{11},{16},{16},{16},{16},{16},{16},{16},{16},{7},{11},{16},{16},{16},{16},{16},{16},{16},{16},{8},{16},{16},{16},{16},{16},{16},{16},{16},{16},{9},{16},{16},{16},{16},{16},{16},{16},{16},{16},{9},{16},{16},{16},{16},{16},{16},{16},{16},{16},{9},{16},{16},{16},{16},{16},{16},{16},{16},{16},{9},{16},{16},{16},{16},{16},{16},{16},{16},{16},{11},{16},{16},{16},{16},{16},{16},{16},{16},{16},{14},{16},{16},{16},{16},{16},{16},{16},{16},{16},{10},{15},{16},{16},{16},{16},{16},{16},{16},{16},{16}, };


void readFileByBytes(const string& filePath, vector<short int> &vec) {
    ifstream file(filePath, ios::binary);
    if (!file.is_open()) {
        cout << "Не удалось открыть файл: " << filePath << endl;
        return;
    }

    char byte;
    while (file.get(byte)) {
        vec.push_back(static_cast<int>(byte));
    }

    file.close();
}

void writeByBytes(const string& filePath, vector<short int>& vec, ofstream &file) {
    /*if (!file.is_open())
    {
        cout << "Не удалось открыть файл: " << filePath << endl;
        return;
    }*/
    for (int i = 0; i < vec.size(); i++) {
        file.write(reinterpret_cast<const char*>(&vec[i]), 1);
    }
    //file.close();
}

int intRound(float n) {
    if (n < 0) {
        n = n * -1;
        short int buffer = n;
        if (n - buffer > 0.5) {
            return (n + 1) * -1;
        }
        else {
            return (n) * -1;
        }
    }
    else {
        short int buffer = n;
        if (n - buffer >= 0.5) {
            return (n + 1);
        }
        else {
            return n;
        }
    }
}

void stringToMatrix(vector <vector <short int>>& matrix, vector <short int>& string, int imageSize) {
    vector <short int> matrix_buffer;

    matrix.resize(imageSize);
    for (int i = 0; i < imageSize; i++) {
        matrix[i].resize(imageSize);
    }

    int counter = 0;
    int matrixLen = string.size() / imageSize;
    for (int i = 0; i < matrixLen; i++) {
        for (int j = 0; j < matrixLen; j++) {
            matrix[j][i] = string[counter];
            counter++;
        }
    }
}

void matrixToString(vector<vector<short int>>& matrix, vector <short int>& string) {
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix.size(); j++) {
            string.push_back(matrix[j][i]);
        }
    }
}

void RLE(vector<short int> string, vector<short int>& encodedString) {
    int len = string.size();
    int zeroCounter = 0;
    int globalCounter = 0;
    for (int i = 0; i < len; i++) {
        if (globalCounter == 63) {
            encodedString.push_back(zeroCounter);
            encodedString.push_back(string[i]);
            zeroCounter = 0;
            globalCounter = 0;
        }
        else {
            if (string[i] != 0) {
                encodedString.push_back(zeroCounter);
                encodedString.push_back(string[i]);
                zeroCounter = 0;
            }
            else {
                zeroCounter++;
            }
            globalCounter++;
        }
    }
    if (zeroCounter != 0) {
        encodedString.push_back(zeroCounter);
        encodedString.push_back(string[len - 1]);
    }
}

void RLEdecoder(vector<short int>string, vector<short int>& decodedString) {
    int len = string.size();
    int counter = 0;
    while (counter < len) {

        for (int i = 0; i < string[counter]; i++) {
            decodedString.push_back(0);
        }
        decodedString.push_back(string[counter + 1]);
        counter += 2;
    }
}

void RGB_to_YCrCb(vector<short int> &rgb, vector<short int> &Y_layer, vector<short int> &Cr_layer, vector<short int> &Cb_layer, bool mode) {
    int rgbCounter = 0;
    int yccCounter = 0;
    //mode: bw - true, rgb - false
    if (!mode) {
        while (rgbCounter < rgb.size()) {
            Y_layer.push_back(0.299 * rgb[rgbCounter] + 0.587 * rgb[rgbCounter + 1] + 0.114 * rgb[rgbCounter + 2]);
            Cb_layer.push_back(- 0.168736 * rgb[rgbCounter] - 0.331264 * rgb[rgbCounter + 1] + 0.5 * rgb[rgbCounter + 2] + 128);
            Cr_layer.push_back(0.5 * rgb[rgbCounter] - 0.418688 * rgb[rgbCounter + 1] - 0.081312 * rgb[rgbCounter + 2] + 128);
            rgbCounter += 3;
        }
    }
    else {
        while (rgbCounter < rgb.size()) {
            Cb_layer.push_back(128);
            Cr_layer.push_back(128);
            Y_layer.push_back(rgb[rgbCounter]);
            rgbCounter++;
        }
    }
    

}

void YCrCb_to_RGB(vector<short int>& Y_layer, vector<short int>& Cr_layer, vector<short int>& Cb_layer, vector<short int>& rgb) {
    int rgbCounter = 0;
    int yccCounter = 0;
    while (yccCounter < Y_layer.size()) {
        rgb.push_back(intRound(Y_layer[yccCounter] + 1.402 * (Cr_layer[yccCounter] - 128)));
        rgb.push_back(intRound(Y_layer[yccCounter] - 0.34414 * (Cb_layer[yccCounter] - 128) - 0.71414 * (Cr_layer[yccCounter] - 128)));
        rgb.push_back(intRound(Y_layer[yccCounter] + 1.772 * (Cb_layer[yccCounter] - 128)));
        yccCounter++;
    }
}

void downsampling(vector<vector<short int>>& Cr_layer, vector<vector<short int>>& Cb_layer) {
    vector<vector<short int>> new_Cr_layer;
    vector<vector<short int>> new_Cb_layer;
    int len = Cr_layer.size();
    int i = 0;
    while (i < len) {
        int j = 0;
        vector<short int > new_Cr_layer_string;
        vector<short int > new_Cb_layer_string;
        while (j < len) {
            new_Cr_layer_string.push_back(Cr_layer[i][j]);
            new_Cb_layer_string.push_back(Cb_layer[i][j]);
            j += 2;
        }
        new_Cr_layer.push_back(new_Cr_layer_string);
        new_Cb_layer.push_back(new_Cb_layer_string);
        i += 2;
    }
    Cr_layer = new_Cr_layer;
    Cb_layer = new_Cb_layer;
}

void upsampling(vector<vector<short int>>& Cr_layer, vector<vector<short int>>& Cb_layer) {
    int len = Cr_layer.size();
    int i = 0;
    vector<vector<short int>> new_Cr_layer;
    vector<vector<short int>> new_Cb_layer;
    for (int i = 0; i < len * 2; i++) {
        vector<short int> buffer;
        for (int j = 0; j < len * 2; j++) {
            buffer.push_back(0);
        }
        new_Cr_layer.push_back(buffer);
        new_Cb_layer.push_back(buffer);
    }
    while (i < len) {
        int j = 0;
        while (j < len) {
            new_Cr_layer[i * 2][j * 2] = Cr_layer[i][j];
            new_Cr_layer[i * 2 + 1][j * 2] = Cr_layer[i][j];
            new_Cr_layer[i * 2][j * 2 + 1] = Cr_layer[i][j];
            new_Cr_layer[i * 2 + 1][j * 2 + 1] = Cr_layer[i][j];

            new_Cb_layer[i * 2][j * 2] = Cb_layer[i][j];
            new_Cb_layer[i * 2 + 1][j * 2] = Cb_layer[i][j];
            new_Cb_layer[i * 2][j * 2 + 1] = Cb_layer[i][j];
            new_Cb_layer[i * 2 + 1][j * 2 + 1] = Cb_layer[i][j];
            j += 1;
        }
        i += 1;
    }
    Cr_layer = new_Cr_layer;
    Cb_layer = new_Cb_layer;
}

float alpha(float number) {
    if (number) {
        return (1 / sqrt(2));
    }
    else {
        return 1;
    }
}

void DCT_II(vector<vector<short int>> matrix, vector<vector<short int>>& matrixDCTed, bool mode, int quality) {
    int MCUlen = 8;
    int len = matrix.size() / MCUlen;
    int matrixLen = matrix.size();
    //mode: Y - false, Cr/Cb - true
    for (int i = 0; i < quantingTableAC.size(); i++) {
        for (int j = 0; j < quantingTableAC.size(); j++) {
            quantingTableAC[i][j] = (quantingTableAC[i][j] / 50.0) * quality;
        }
    }
    for (int i = 0; i < quantingTableDC.size(); i++) {
        for (int j = 0; j < quantingTableDC.size(); j++) {
            quantingTableDC[i][j] = (quantingTableDC[i][j] / 50.0) * quality;
        }
    }

    vector<short int> buffer;
    matrixDCTed.resize(matrixLen);
    for (int i = 0; i < matrixLen; i++) {
        matrixDCTed[i].resize(matrixLen);
    }
    for (int i = 0; i < len * MCUlen; i++) {
        for (int j = 0; j < len * MCUlen; j++) {
            matrix[i][j] -= 128;
        }
    }
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {
            for (int u = 0; u < MCUlen; u++) {
                for (int v = 0; v < MCUlen; v++) {
                    float summPart = 0;
                    for (int x = 0; x < MCUlen; x++) {
                        for (int y = 0; y < MCUlen; y++) {
                            summPart += matrix[x + i * MCUlen][y + j * MCUlen] * cos(((2 * x + 1) * u * M_PI) / 16) * cos(((2 * y + 1) * v * M_PI) / 16);
                        }
                    }
                    int buf = 0;
                    if (mode) {
                        buf = intRound(((alpha(u) * alpha(v) / 4) * summPart) / quantingTableAC[u][v]);
                        matrixDCTed[u + i * MCUlen][v + j * MCUlen] = buf;
                    }
                    else {
                        buf = intRound(((alpha(u) * alpha(v) / 4) * summPart) / quantingTableDC[u][v]);
                        matrixDCTed[u + i * MCUlen][v + j * MCUlen] = buf;
                    }
                }
            }
        }
    }
    /*cout << "\n";
    for (int i = 0; i < matrixDCTed.size(); i++) {
        if (i % 8 == 0) {
            cout << "\n";
        }
        for (int j = 0; j < matrixDCTed[0].size(); j++) {
            if (j % 8 == 0) {
                cout << "\t\t\t";
            }
            cout << matrixDCTed[i][j] << "|";
        }
        cout << "\n";
    }*/
}

void DCT_III(vector<vector<short int>> matrix, vector<vector<short int>>& matrixDCTed, bool mode) {
    int MCUlen = 8;
    int len = matrix.size() / MCUlen;
    int matrixLen = matrix.size();
    vector<short int> buffer;
    matrixDCTed.resize(matrixLen);
    for (int i = 0; i < matrixLen; i++) {
        matrixDCTed[i].resize(matrixLen);
    }
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {
            for (int x = 0; x < MCUlen; x++) {
                for (int y = 0; y < MCUlen; y++) {
                    if (mode) {
                        matrix[x + i * MCUlen][y + j * MCUlen] = intRound(matrix[x + i * MCUlen][y + j * MCUlen] * quantingTableAC[x][y]);
                    }
                    else {
                        matrix[x + i * MCUlen][y + j * MCUlen] = intRound(matrix[x + i * MCUlen][y + j * MCUlen] * quantingTableDC[x][y]);
                    }
                }
            }
        }
    }
    for (int i = 0; i < len; i++) {
        for (int jndex = 0; jndex < len; jndex++) {
            for (int x = 0; x < MCUlen; x++) {
                for (int y = 0; y < MCUlen; y++) {
                    float summPart = 0;
                    for (int u = 0; u < MCUlen; u++) {
                        for (int v = 0; v < MCUlen; v++) {
                            summPart += alpha(u) * alpha(v) * matrix[u + i * MCUlen][v + jndex * MCUlen] * cos(((2 * x + 1) * u * M_PI) / 16) * cos(((2 * y + 1) * v * M_PI) / 16);
                        }
                    }
                    int buff = intRound(summPart / 4);
                    matrixDCTed[x + i * MCUlen][y + jndex * MCUlen] = buff;
                }

            }
        }
    }
    for (int i = 0; i < len * MCUlen; i++) {
        for (int j = 0; j < len * MCUlen; j++) {
            matrixDCTed[i][j] += 128;
        }
    }
    /*for (int i = 0; i < matrixDCTed.size(); i++) {
        if (i % 8 == 0) {
            cout << "\n";
        }
        for (int j = 0; j < matrixDCTed[0].size(); j++) {
            if (j % 8 == 0) {
                cout << "\t\t\t";
            }
            cout << matrixDCTed[i][j] << "|";
        }
        cout << "\n";
    }*/
}

void differentialEnc(vector<vector<short int>>& matrix) {
    int len = matrix.size();
    int lastElem;
    int buffer;
    for (int i = 0; i < len; i += 8) {
        for (int j = 0; j < len; j += 8) {
            if (i == 0 and j == 0) {
                buffer = matrix[i][j];
                matrix[i][j] = matrix[i][j];
                lastElem = buffer;
            }
            else {
                buffer = matrix[i][j];
                matrix[i][j] = matrix[i][j] - lastElem;
                lastElem = buffer;
            }

        }
    }
}

void differentialDec(vector<vector<short int>>& matrix) {
    int len = matrix.size();
    int lastElem;
    for (int i = 0; i < len; i += 8) {
        for (int j = 0; j < len; j += 8) {
            if (i == 0 and j == 0) {
                matrix[i][j] = matrix[i][j];
                lastElem = matrix[i][j];
            }
            else {
                matrix[i][j] = matrix[i][j] + lastElem;
                lastElem = matrix[i][j];
            }

        }
    }
}

void zigzag(vector<vector<short int>> matrix, vector<short int>& string) {
    int matrixLen = matrix.size() / 8;
    int indexesLen = zigzagIndexes.size();
    for (int i = 0; i < matrixLen; i++) {
        for (int j = 0; j < matrixLen; j++) {
            for (int index = 0; index < indexesLen; index++) {
                string.push_back(matrix[i * 8 + zigzagIndexes[index][0]][j * 8 + zigzagIndexes[index][1]]);
            }
        }
    }
}

void zigzagReversed(vector<short int> string, vector<vector<short int>>& matrix, int size) {
    int matrixLen = size / 8;
    int indexesLen = zigzagIndexes.size();
    matrix.resize(size);
    for (int i = 0; i < size; i++) {
        matrix[i].resize(size);
    }
    int stringCounter = 0;
    for (int i = 0; i < matrixLen; i++) {
        for (int j = 0; j < matrixLen; j++) {
            for (int index = 0; index < indexesLen; index++) {
                matrix[i * 8 + zigzagIndexes[index][0]][j * 8 + zigzagIndexes[index][1]] = string[stringCounter];
                stringCounter++;
            }
        }
    }
}

    int main() {
        vector<short int> rawVec;
        vector<short int> Y_layer;
        vector<short int> Cr_layer;
        vector<short int> Cb_layer;
        vector<short int> res_rgb;
        vector<vector<short int>> Y_mat;
        vector<vector<short int>> Cr_mat;
        vector<vector<short int>> Cb_mat;
        vector<vector<short>> dublevec;
        vector<vector<short>> dublevec_new;
        vector<vector<float>> dctvec;

        string inputFile = "C:\\Users\\aasto\\source\\repos\\AISTlab2sem_2lab\\rawf\\test_image.raw";
        string ouputFile = "C:\\Users\\aasto\\source\\repos\\AISTlab2sem_2lab\\test\\test_image_20.raw";
        readFileByBytes(inputFile, rawVec);
        RGB_to_YCrCb(rawVec, Y_layer, Cr_layer, Cb_layer, RGB_MODE);
        vector<vector<short int>> Y_matrix;
        vector<vector<short int>> Cb_matrix;
        vector<vector<short int>> Cr_matrix;

        stringToMatrix(Cr_matrix, Cr_layer, 2048);
        stringToMatrix(Cb_matrix, Cb_layer, 2048);
        stringToMatrix(Y_matrix, Y_layer, 2048);

        downsampling(Cr_matrix, Cb_matrix);

        vector<vector<short int>> Y_matrix_dct;
        vector<vector<short int>> Cb_matrix_dct;
        vector<vector<short int>> Cr_matrix_dct;
        DCT_II(Cr_matrix, Cr_matrix_dct, CR_CB_MODE, QUANT_QUALITY);
        DCT_II(Cb_matrix, Cb_matrix_dct, CR_CB_MODE, QUANT_QUALITY);
        DCT_II(Y_matrix, Y_matrix_dct, Y_MODE, QUANT_QUALITY);

        differentialEnc(Cr_matrix_dct);
        differentialEnc(Cb_matrix_dct);
        differentialEnc(Y_matrix_dct);

        vector<short int> Cr_matrix_dct_zigzag;
        vector<short int> Cb_matrix_dct_zigzag;
        vector<short int> Y_matrix_dct_zigzag;
        zigzag(Cr_matrix_dct, Cr_matrix_dct_zigzag);
        zigzag(Cb_matrix_dct, Cb_matrix_dct_zigzag);
        zigzag(Y_matrix_dct, Y_matrix_dct_zigzag);
        vector<short int> Cr_rle;
        vector<short int> Cb_rle;
        vector<short int> Y_rle;
        RLE(Cr_matrix_dct_zigzag, Cr_rle);
        RLE(Cb_matrix_dct_zigzag, Cb_rle);
        RLE(Y_matrix_dct_zigzag, Y_rle);

        Cb_matrix_dct_zigzag.clear();
        Cr_matrix_dct_zigzag.clear();
        Y_matrix_dct_zigzag.clear();

        RLEdecoder(Y_rle, Y_matrix_dct_zigzag);
        RLEdecoder(Cr_rle, Cr_matrix_dct_zigzag);
        RLEdecoder(Cb_rle, Cb_matrix_dct_zigzag);

        Y_matrix_dct.clear();
        Cr_matrix_dct.clear();
        Cb_matrix_dct.clear();

        zigzagReversed(Y_matrix_dct_zigzag, Y_matrix_dct, 2048);
        zigzagReversed(Cr_matrix_dct_zigzag, Cr_matrix_dct, 1024);
        zigzagReversed(Cb_matrix_dct_zigzag, Cb_matrix_dct, 1024);
        differentialDec(Cr_matrix_dct);
        differentialDec(Cb_matrix_dct);
        differentialDec(Y_matrix_dct);

        vector<vector<short int>> Y_matrix_back_dct;
        vector<vector<short int>> Cb_matrix_back_dct;
        vector<vector<short int>> Cr_matrix_back_dct;
        DCT_III(Cr_matrix_dct, Cr_matrix_back_dct, CR_CB_MODE);
        DCT_III(Cb_matrix_dct, Cb_matrix_back_dct, CR_CB_MODE);
        DCT_III(Y_matrix_dct, Y_matrix_back_dct, Y_MODE);

        upsampling(Cr_matrix, Cb_matrix);

        Y_layer.clear();
        Cr_layer.clear();
        Cb_layer.clear();
        matrixToString(Y_matrix, Y_layer);
        matrixToString(Cr_matrix, Cr_layer);
        matrixToString(Cb_matrix, Cb_layer);

        YCrCb_to_RGB(Y_layer, Cr_layer, Cb_layer, res_rgb);
        ofstream file(ouputFile, ios::binary);
        writeByBytes(ouputFile, res_rgb, file);
    }
