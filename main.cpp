#include <iostream>
#include <fstream>
#include "lfsr_lib.h"
#include <cmath>

void print_sequence(lfsr_lib lfsr, const char *fname);

bool Geffa(lfsr_lib &r1, lfsr_lib &r2, lfsr_lib &r3);

bool freq_test(std::vector<int> seq, unsigned int n);

double series_test(std::vector<int> seq, std::vector<int> &subseq1, std::vector<int> &subseq2, unsigned int n);

int main() {

    int seed1 = 7, seed2 = 1, seed3 = 15;

    lfsr_lib lfsr_1({1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1}, seed1); //x^17 + x^3 + 1  | geffa 1

    lfsr_lib lfsr_2({1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
                    seed2); //x^17 + x^5 + 1  | geffa (1+1) mod 6

    lfsr_lib lfsr_3({1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1}, seed3); //x^12 + x^6 + x^4 + x + 1  | lfsr (1-2) mod 6

    print_sequence(lfsr_1, "lfsr_1.txt");
    print_sequence(lfsr_2, "lfsr_2.txt");
    print_sequence(lfsr_3, "lfsr_3.txt");


    char fname[40] = "out.txt";
//    std::cin >> fname;
    std::cout << "output file name: " << fname << std::endl;

    std::vector<int> sequence(0);
    std::ofstream file(fname);
    int period = 1;

    std::cout << std::endl;

    do {
        bool bit = Geffa(lfsr_1, lfsr_2, lfsr_3);
        sequence.push_back(bit);
        file << bit << std::endl;
        lfsr_1.shift();
        lfsr_2.shift();
        lfsr_3.shift();
        period++;
    } while (((lfsr_1.get_period() != 1) || (lfsr_3.get_period() != 1) || (lfsr_2.get_period() != 1)) && period < 30);

    file.close();

    bool res_freq = freq_test(sequence, period);
    std::cout << std::endl;
    std::cout << "Период: " << period << std::endl << std::endl;

    std::vector<int> subseq_0(0);
    std::vector<int> subseq_1(0);

    double Hi = series_test(sequence, subseq_0, subseq_1, period);

    std::cout << "Максимальная последовательность '0' : " << subseq_0.size() << std::endl;
    std::cout << "Максимальная последовательность '1' : " << subseq_1.size() << std::endl << std::endl;

    for (int i = 0; i < subseq_0.size(); i++) {
        std::cout << "Количество '0";
        for (int j = 0; j < i; j++)
            std::cout << "0";
        std::cout << "' : " << subseq_0[i] << std::endl;
    }
    std::cout << std::endl;

    for (int i = 0; i < subseq_1.size(); i++) {
        std::cout << "Количество '1";
        for (int j = 0; j < i; j++)
            std::cout << "1";
        std::cout << "' : " << subseq_1[i] << std::endl;
    }
    std::cout << std::endl;

    Hi = series_test(sequence, subseq_0, subseq_1, 1000);

    std::cout << "Тест серий, Hi^2: " << Hi << std::endl;

    return 0;
}

void print_sequence(lfsr_lib lfsr, const char *fname) {
    std::ofstream file(fname);

    file << "Start: " << lfsr.get_start_val() << std::endl;

    do {
        file << lfsr.get_bit() << " Period: " << lfsr.get_period() << std::endl;
        lfsr.shift();
    } while (lfsr.get_period() > 1);
    file << lfsr.get_bit() << " Period: " << lfsr.get_period() << std::endl;

    file << "Degree: " << lfsr.get_degree() << std::endl;
}

bool Geffa(lfsr_lib &r1, lfsr_lib &r2, lfsr_lib &r3) {
    bool res = (r1.get_bit() & r3.get_bit()) ^ (~r3.get_bit() & r2.get_bit()); //r1r3^r2~r3
    return res;
}


bool freq_test(std::vector<int> seq, unsigned int n) {
    int f = 0;
    for (unsigned int i = 0; i < n - 1; i++)
        f += seq[i];
    std::cout << "Количество '1': " << f << std::endl;
    std::cout << "Количество '0': " << n - f << std::endl;
    f = f - (n / 2);
    double ff = f * 2 / sqrt(n);

    if ((-3 < ff) && (ff < 3))
        return true;
    else
        return false;
}

double series_test(std::vector<int> seq, std::vector<int> &subseq1, std::vector<int> &subseq2, unsigned int n) {
    subseq1.clear();
    subseq2.clear();
    subseq1.resize(n);
    subseq2.resize(n);

    {
        int len = 1, zero = 2;
        for (int i = 0; i < n - 1; i++) {
            if (seq[i] == 0) {
                if (zero != 1) {
                    subseq2[len - 1]++;
                    zero = 1;
                    len = 1;
                } else {
                    len++;
                }
            } else {
                if (zero != 0) {
                    subseq1[len - 1]++;
                    zero = 0;
                    len = 1;
                } else {
                    len++;
                }
            }
        }
    }

    while (subseq1[subseq1.size() - 1] == 0)
        subseq1.pop_back();
    while (subseq2[subseq2.size() - 1] == 0)
        subseq2.pop_back();

    double Hi = 0;
    if (subseq1.size() > subseq2.size()) {
        for (int i = 0; i < subseq1.size() - 1; i++) {
            double divider = pow(2, i + 1 + 2); //divider
            double iter = (subseq1[i] - (n / divider)) * (subseq1[i] - (n / divider)) / (n / divider);
            Hi += iter;
        }
    } else {
        for (int i = 0; i < subseq2.size() - 1; i++) {
            double divider = pow(2, i + 1 + 2); //divider
            double iter = (subseq2[i] - (n / divider)) * (subseq2[i] - (n / divider)) / (n / divider);
            Hi += iter;
        }
    }

    return Hi;
}