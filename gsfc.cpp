/* Written by Dan Hemphill 11/10/2018 daniel_hemphill@student.uml.edu
 * Compiled with g++
 */

#include<cstring>
#include<string>
#include<iostream>
#include<iomanip>
#include<fstream>
#include<sstream>
#include<vector>
#include<iterator>
#include<algorithm>
#include<cmath>

const int REPELEMS = 60;

void trim (std::vector<float>& v, int trim) {
  int trimSize = ((v.size()/100.0) * (trim/2.0));
  v.erase(v.begin(), v.begin() + (trimSize));
  v.resize(v.size() - trimSize);
}
float mean(std::vector<float>& v) {
  float average = 0;
  for (std::vector<float>::iterator it = v.begin(); it != v.end(); ++it)
      average += *it;
    average /= v.size();
  return average;
}
float stdev(std::vector<float>& v, float mean) {
  float deviation = 0;
  for (std::vector<float>::iterator it = v.begin(); it != v.end(); ++it)
    deviation += ((*it - mean) * (*it - mean));
  deviation /= v.size();
  deviation = sqrt(deviation);
  return deviation;
}
int main(int argc, char **argv) {
  // %of elements to trim
  int TRIM = 20;
  if (argc < 2)
    throw std::invalid_argument
      ("syntax: ./force_mean_calculator [path_to_file]\n");
  
  std::vector<std::string> Ft;
  std::vector<std::string> F0;
  std::vector<std::string> F1;
  std::string cell, line, filename;
  filename = argv[1];
  if (argc >= 3)
    TRIM = atoi(argv[2]);
  std::ifstream file;
  file.open(filename.c_str());
  while (std::getline(file, line)) {
    std::stringstream ss(line);
    std::getline(ss, cell, ',');
    F0.push_back(cell);
    std::getline(ss, cell, ',');
    F1.push_back(cell);
    std::getline(ss, cell, ',');
    Ft.push_back(cell);
  }
  
  std::vector<float> trial;
  // starts at the 10th row of the .csv
  for (int i = 10; i < Ft.size(); ++i)
    trial.push_back(atof(Ft[i].c_str()));

  // gets index of starting cell of each repetition
  std::vector<int> indices;
  for (std::vector<float>::iterator it = (trial.begin() + 1);
       it != (trial.end() - REPELEMS); ++it) {
    bool isConstantForce = true;
    if (*(it - 1) == 0 && *it != 0) {
      for (std::vector<float>::iterator ik = it; ik != (it + REPELEMS); ++ik)
	if (*ik == 0) {
	  isConstantForce = false;
	  break;
	}
      if (isConstantForce)
	indices.push_back(it - trial.begin());
    }
  }
  
  std::vector<std::vector<float> > repetitions;
  for (std::vector<int>::iterator it = indices.begin();
      it != indices.end(); ++it) {
    std::vector<float> rep;
    int i = *it;
    do {
      rep.push_back(trial[i]);
      ++i;
    }while(trial[i] != 0);
    repetitions.push_back(rep);
  }
  
  for(std::vector<std::vector<float> >::iterator it = repetitions.begin();
      it != repetitions.end(); ++it)
    trim(*it, TRIM);
  
  std::vector<float> repMeanValues;
  for (std::vector<std::vector<float> >::iterator it = repetitions.begin();
       it != repetitions.end(); ++it)
    repMeanValues.push_back(mean(*it));
  
  std::vector<float> repStdValues;
  for (int i = 0; i < repetitions.size(); ++i) {
    repStdValues.push_back(stdev(repetitions[i], repMeanValues[i]));
  }

  std::vector<float> trialMeanValues = repMeanValues;
  std::sort(trialMeanValues.begin(), trialMeanValues.end());
  trim(trialMeanValues, TRIM);
  float trialMean = mean(trialMeanValues);
  float dev = stdev(trialMeanValues, trialMean);
  
  char *cstr = new char[filename.length() + 1];
  std::strcpy(cstr, filename.c_str());
  std::string outfile = basename(cstr);
  outfile.resize(outfile.size() - 4);
  std::ofstream output(std::string("./data/") + outfile + ("_data.txt"));
  delete [] cstr;

  output << "      mean      \u03C3\n";
  output << std::fixed << std::setprecision(3);
  for (int i = 0; i < repMeanValues.size(); ++i) {
    if (i < 9)
      output << "[" << i+1 << "]   " << repMeanValues[i]
	     << "   " << repStdValues[i] << "\n";
    else
      output << "[" << i+1 << "]  " << repMeanValues[i]
	     << "   " << repStdValues[i] << "\n";
  }
  output << "\n" << "trial mean @ "<< TRIM
  	 << "% trim: " << trialMean << "\n"
  	 << "\u03C3: " << dev;
  output.close();

  return 0;
}
