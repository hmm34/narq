/*
@file test.cpp
@brief Unit test and timing tests for narq
@author Drew Guarnera, Heather Michaud
*/

#include "../../src/tools.hpp"
#include <fstream>
#include <iostream>
#include <ctime>

const std::string outputFile_hup = "timing_increasing_haystack.csv";
const std::string outputFile_nup = "timing_increasing_needle.csv";

const std::string outputFile_multi_hup = "timing_increasing_multi_haystack.csv";
const std::string outputFile_multi_nup = "timing_increasing_multi_needle.csv";

const int MIN_NUM_N = 10;  // Minimum number of needles
const int MAX_NUM_N = 100; // Maximum number of needles
const int MIN_N = 10;      // Minimum needle string size
const int MAX_N = 1000;    // Maximum needle string size
const int MIN_H = 100;     // Minimum haystack string size
const int MAX_H = 10000;   // Maximum haystack string size
const int INCREMENT = 100; // How much the needle & haystack are incremented

//! @brief Generates a string with all values of a letter, used te test the worst case scenario
//! @param n The length of the string to be generated
//! @retval Generated sequence of letters of length n
std::string generateString(long long n, char letter);

//! @brief Executes timing tests on naive and rabin karp algorithm, varying the size of the needle 
//! to search for
void varyNeedleSize();

//! @brief Executes timing tests on naive and rabin karp algorithm, varying the size of multiple needles 
//! to search for
void varyMultipleNeedleSize();

//! @brief Executes timing tests on naive and rabin karp algorithm, varying the size of the 
//! haystack to search in
void varyHaystackSize();

//! @brief Executes timing tests on naive and rabin karp algorithm, varying the number of needles and size of the 
//! haystack to search in
void varyMultipleHaystackSize();

int main(int argc, char const *argv[])
{
	varyNeedleSize();
	varyHaystackSize();
	varyMultipleNeedleSize();
	varyMultipleHaystackSize();
	return 0;
}

// ------------------------------------- Testing Utilities -------------------------------------- 
std::string generateString(long long n, char letter)
{
	std::string sequence = "";
	for (int i = 0; i < n; ++i)
		sequence += letter;
	return sequence;
}

void varyNeedleSize() {
	// Open output file and write header column names
	std::ofstream output(outputFile_nup.c_str());
	if (!output)
	{
		std::cerr << "Could not open results file " << outputFile_nup << "\n";
		return;
	}
	output << "needle size,"          << "haystack size,"
	       << "naive time (ms),"      << "index,"
	       << "rabin karp - LV (ms)," << "index,"
	       << "rabin karp - MC (ms)," << "index\n";

	// Varying needle size in a large haystack
	std::cout << "\n";
	std::string haystack = generateString(MAX_H, 'a');
	for (int i = MIN_N; i <= MAX_N; i += INCREMENT)
	{
		int percentComplete = ((i / (double) MAX_N) * 100);
		std::cout << "Testing varying needle size: " << percentComplete << "% " << "\r";
		fflush(stdout);

		std::string needle = generateString(i-1, 'a');
		needle += "b";

		// Time Naive brute force algorithm
		std::clock_t start1 = std::clock();
		int index1 = narq::bruteForce(needle, haystack);
		std::clock_t end1 = std::clock();

		// Time Las Vegas Rabin Karp algorithm
		std::clock_t start2 = std::clock();
		int index2 = narq::rabinKarpLV(needle, haystack);
		std::clock_t end2 = std::clock();

		// Time Monte Carlo Rabin Karp algorithm
		std::clock_t start3 = std::clock();
		int index3 = narq::rabinKarpMC(needle, haystack);
		std::clock_t end3 = std::clock();

		// Calculate and output time differences and found index. All indices should be -1 because 
		// the sequence was not found.
		double msTotal1 = 1000.0 * (end1 - start1) / CLOCKS_PER_SEC;
		double msTotal2 = 1000.0 * (end2 - start2) / CLOCKS_PER_SEC;
		double msTotal3 = 1000.0 * (end3 - start3) / CLOCKS_PER_SEC;
		output << i        << "," << MAX_H  << ","
		       << msTotal1 << "," << index1 << ","
		       << msTotal2 << "," << index2 << ","
		       << msTotal3 << "," << index3 << "\n";
	}
	std::cout << "Testing varying needle size: " << 100 << "% " << "\r";
	std::cout << "\n";
	output.close();
}

void varyHaystackSize() {
	// Open output file and write header column names
	std::ofstream output(outputFile_hup.c_str());
	if (!output)
	{
		std::cerr << "Could not open results file " << outputFile_hup << "\n";
		return;
	}
	output << "needle size,"          << "haystack size,"
	       << "naive time (ms),"      << "index,"
	       << "rabin karp - LV (ms)," << "index,"
	       << "rabin karp - MC (ms)," << "index\n";

	std::string needle = generateString(MIN_N - 1, 'a');
	needle += "b";

	// For varying levels of haystack size (string text body to search in)
	for (int i = MIN_H; i <= MAX_H; i += INCREMENT)
	{
		int percentComplete = (i / (double) MAX_H) * 100;
		std::cout << "Testing varying haystack size: " << percentComplete << "%" << "\r";
		fflush(stdout);

		std::string haystack = generateString(i, 'a');

		// Time Naive brute force algorithm
		std::clock_t start1 = std::clock();
		int index1 = narq::bruteForce(needle, haystack);
		std::clock_t end1 = std::clock();

		// Time Las Vegas Rabin Karp algorithm
		std::clock_t start2 = std::clock();
		int index2 = narq::rabinKarpLV(needle, haystack);
		std::clock_t end2 = std::clock();

		// Time Monte Carlo Rabin Karp algorithm
		std::clock_t start3 = std::clock();
		int index3 = narq::rabinKarpMC(needle, haystack);
		std::clock_t end3 = std::clock();

		// Calculate and output time differences and found index. All indices should be -1 because 
		// the sequence was not found.
		double msTotal1 = 1000.0 * (end1 - start1) / CLOCKS_PER_SEC;
		double msTotal2 = 1000.0 * (end2 - start2) / CLOCKS_PER_SEC;
		double msTotal3 = 1000.0 * (end3 - start3) / CLOCKS_PER_SEC;
		output << MIN_N    << "," << i      << ","
		       << msTotal1 << "," << index1 << ","
		       << msTotal2 << "," << index2 << ","
		       << msTotal3 << "," << index3 << "\n";
	}
	std::cout << "Testing varying haystack size: " << 100 << "%" << "\r";
	std::cout << "\n";
	output.close();
}

void varyMultipleNeedleSize() {
	// Open output file and write header column names
	std::ofstream output(outputFile_multi_nup.c_str());
	if (!output)
	{
		std::cerr << "Could not open results file " << outputFile_nup << "\n";
		return;
	}
	output << "needle count,"
		   << "needle size,"          << "haystack size,"
	       << "rabin karp multi (ms),"      << "index\n";

	// Varying needle size in a large haystack
	std::string haystack = generateString(MAX_H, 'a');
	int numNeedles = MIN_NUM_N;
	while (numNeedles <= MAX_NUM_N) {
		
		int percentComplete = ((numNeedles / (double) MAX_N) * 100);
		std::cout << "Testing varying number of needles and size: " << percentComplete << "% " << "\r";
		fflush(stdout);

		for (int i = MIN_N; i <= MAX_N; i += INCREMENT)
		{
			std::vector<std::string> needles;

			for (int j = 0; j<= numNeedles; j += 1)
			{
				needles.push_back(generateString(j-1, 'a') + "b");
			}

			std::clock_t start1 = std::clock();
			std::vector<int> index1 = narq::rabinKarpMulti(needles, haystack, needles.size());
			std::clock_t end1 = std::clock();

			int num_matches = 0;
			for (size_t p = 0; p < index1.size(); ++p) {
				num_matches += index1[p];
			}

			// Calculate and output time differences and found index. All indices should be -1 because 
			// the sequence was not found.
			double msTotal1 = 1000.0 * (end1 - start1) / CLOCKS_PER_SEC;
			output << numNeedles << ","
				   << i        << "," << MAX_H  << ","
			       << msTotal1 << "," << num_matches
			       << "\n";
		}

		numNeedles += 10;
	}
	std::cout << "Testing varying number of needles and size: " << 100 << "% " << "\r";
	std::cout << "\n";
	output.close();
}

void varyMultipleHaystackSize() {
	// Open output file and write header column names
	std::ofstream output(outputFile_multi_hup.c_str());
	if (!output)
	{
		std::cerr << "Could not open results file " << outputFile_hup << "\n";
		return;
	}
	output << "needle count, "
		   << "needle size, "          << "haystack size, "
	       << "rabin karp multi (ms)," << "index\n";

	int numNeedles = MIN_NUM_N;
	while (numNeedles <= MAX_NUM_N) {	

		int percentComplete = ((numNeedles / (double) MAX_N) * 100);
		std::cout << "Testing varying needles and haystack size: " << percentComplete << "% " << "\r";
		fflush(stdout);

		std::vector<std::string> needles;

		for (int j = 0; j<= numNeedles; j += 1)
		{
			needles.push_back(generateString(MIN_N - 1, 'a') + "b");
		}
		
		// For varying levels of haystack size (string text body to search in)
		for (int i = MIN_H; i <= MAX_H; i += INCREMENT)
		{
			std::string haystack = generateString(i, 'a');
			
			std::clock_t start1 = std::clock();
			std::vector<int> index1 = narq::rabinKarpMulti(needles, haystack, needles.size());
			std::clock_t end1 = std::clock();

			int num_matches = 0;
			for (size_t p = 0; p < index1.size(); ++p) {
				num_matches += index1[p];
			}

			// Calculate and output time differences and found index. All indices should be -1 because 
			// the sequence was not found.
			double msTotal1 = 1000.0 * (end1 - start1) / CLOCKS_PER_SEC;
			output << numNeedles << ","
				   << MIN_N    << "," << i      << ","
			       << msTotal1 << "," << num_matches << "\n";
		}
		numNeedles += 10;
	}
	std::cout << "Testing varying needles and haystack size: " << 100 << "%" << "\r";
	std::cout << "\n";
	output.close();
}
