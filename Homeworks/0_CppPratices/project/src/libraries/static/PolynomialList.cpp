#include "PolynomialList.h"

#include<iostream>
#include<fstream>  //读取文件
#include<assert.h>
#include <algorithm>
#include<cmath>  //1e-10;


using namespace std;

#define EPSILON 1.0e-10

PolynomialList::PolynomialList(const PolynomialList& other) {
    
    m_Polynomial = other.m_Polynomial;
}

PolynomialList::PolynomialList(const string& file) {
    
    ReadFromFile(file);
}

PolynomialList::PolynomialList(const double* cof, const int* deg, int n) {
   
    for (int i = 0; i < n; i++)
        AddOneTerm(Term(deg[i], cof[i]));
}

PolynomialList::PolynomialList(const vector<int>& deg, const vector<double>& cof) {

    assert(deg.size() == cof.size());
    for (int i = 0; i < deg.size(); i++)
        AddOneTerm(Term(deg[i], cof[i]));
}

double PolynomialList::coff(int i) const {
    for (const Term& term : m_Polynomial) {
        if (term.deg == i) return term.cof;
        if (term.deg > i) break; //???
    }
    return 0.;
}

double& PolynomialList::coff(int i) {

    return AddOneTerm(Term(i, 0)).cof;
}

void PolynomialList::compress() {
    auto itr = m_Polynomial.begin();
    while (itr != m_Polynomial.end())
    {
        if (fabs((*itr).cof )< EPSILON)   //cmath
            itr = m_Polynomial.erase(itr);
        else
            itr++;
    }
}

PolynomialList PolynomialList::operator+(const PolynomialList& right) const {
    PolynomialList poly(*this);
    for (const auto& term : right.m_Polynomial)
        poly.AddOneTerm(term);
    poly.compress();
    return poly; // you should return a correct value
}

PolynomialList PolynomialList::operator-(const PolynomialList& right) const {
    PolynomialList poly(*this);
    for (const auto& term : right.m_Polynomial)
        poly.AddOneTerm(Term(term.deg,-term.cof));

    poly.compress();
    return poly; // you should return a correct value
}

PolynomialList PolynomialList::operator*(const PolynomialList& right) const { //每项相乘再相加
    PolynomialList rst;
    for (const auto& term1 : m_Polynomial) {
        for (const auto& term2 : right.m_Polynomial) {
            double cof = term1.cof * term2.cof;
            int deg = term1.deg + term2.deg;
            rst.AddOneTerm(Term(deg, cof));
        }
    }
    rst.compress();
    return rst; // you should return a correct value
}

PolynomialList& PolynomialList::operator=(const PolynomialList& right) {
    m_Polynomial = right.m_Polynomial;
    return *this;
}

void PolynomialList::Print() const {
    
    auto itr = m_Polynomial.begin();
    if (itr == m_Polynomial.end())
    {
        cout << "0" << endl;
        return;
    }
    for (; itr != m_Polynomial.end(); itr++) {
        if (itr != m_Polynomial.begin()) {
            cout << " ";
            if ((*itr).cof > 0)
                cout << "+";
        }
        cout << itr->cof;
        if (itr->deg > 0)
            cout << "x^" << itr->deg;
    }
    cout << endl;
}

bool PolynomialList::ReadFromFile(const string& file) {
    m_Polynomial.clear();

    ifstream infile;
    infile.open(file.c_str());
    if (!infile.is_open()) {
        cout << "ERROR::PolynomialList::ReadFromFile:" << endl << "\t" << "file[" << file << "]opens failed";
        return false;
    }
    char ch;
    int n;
    infile >> ch;
    infile >> n; //读入第一行
    for (int i = 0; i < n; i++) {
        Term nd;
        infile >> nd.deg;
        infile >> nd.cof;

        AddOneTerm(nd); //加入项nd
    }
    infile.close();
    
    return true;
}

PolynomialList::Term& PolynomialList::AddOneTerm(const Term& term) {
    auto itr = m_Polynomial.begin();
    for (; itr != m_Polynomial.end(); itr++) //auto自动判断类型
    {
        if (itr->deg = term.deg) {
            itr->cof += term.cof;
            return *itr;
        }
        if (itr->deg > term.deg) break;
    }
    return *m_Polynomial.insert(itr, term);
}
