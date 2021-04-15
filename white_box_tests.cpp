//======== Copyright (c) 2021, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - Tests suite
//
// $NoKeywords: $ivs_project_1 $white_box_code.cpp
// $Author:     Vojtech Eichler <xeichl01@stud.fit.vutbr.cz>
// $Date:       $2021-02-26
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author Vojtech Eichler
 * 
 * @brief Implementace testu prace s maticemi.
 */

#include "gtest/gtest.h"
#include "white_box_code.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy operaci nad maticemi. Cilem testovani je:
// 1. Dosahnout maximalniho pokryti kodu (white_box_code.cpp) testy.
// 2. Overit spravne chovani operaci nad maticemi v zavislosti na rozmerech 
//    matic.
//============================================================================//
class MatrixTests : public ::testing::Test
{
protected: 
    virtual void SetUp() {
        m1.set({{3, 5, 1},
                {8, 2, 4},
                {9, 6, 7}});

        m2.set({{8, 2, 4},
                {3, 5, 1},
                {9, 6, 7}});

        m3.set({{1, 7},
                {9, 8},
                {2, 6},
                {5, 3}});

        m4.set({{8, 2, 0, 5},
                {3, 6, 4, 1}});
    } 
    Matrix m1 = Matrix(3, 3);
    Matrix m2 = Matrix(3, 3);
    Matrix m3 = Matrix(4, 2);
    Matrix m4 = Matrix(2, 4);
    Matrix m5 = Matrix();
};

TEST_F(MatrixTests, Get) {
    EXPECT_EQ(m1.get(0,0),3);
    EXPECT_ANY_THROW(m1.get(6,6));
}

TEST_F(MatrixTests, Set) {
    EXPECT_ANY_THROW(Matrix inv = Matrix(0,0));

    EXPECT_TRUE(m1.set(0,0,33));
    EXPECT_EQ(m1.get(0,0), 33);

    EXPECT_FALSE(m1.set(6, 6, 33));

    EXPECT_TRUE(m1.set({{1, 3, 5}, {4, 8, 2}, {7, 9, 6}}));
    EXPECT_EQ(m1.get(0, 0), 1);
    //testing setting 8 values into a matrix of size == 9
    //if this test would fail, I can't continue
    EXPECT_FALSE(m1.set({{1, 3, 5}, {4, 8, 2}, {7, 9}}));
    //inserting correct number of values but only in one line
    EXPECT_FALSE(m1.set({{1, 3, 5, 4, 8, 2, 7, 9, 6}}));
}

TEST_F(MatrixTests, EqualOperator) {
    //two matrices of different sizes
    EXPECT_ANY_THROW(m1 == m3);
    
    EXPECT_TRUE(m1 == m1);
    //matrices of same size, but different values
    EXPECT_FALSE(m1 == m2);
}

TEST_F(MatrixTests, PlusOperator) {
    //two matrices of different sizes
    EXPECT_ANY_THROW(m1 + m3);
    
    ASSERT_NO_THROW(m1 = m1 + m2);
    EXPECT_EQ(m1.get(0,0), 11);
    EXPECT_EQ(m1.get(1,1), 7);
    EXPECT_EQ(m1.get(2,2), 14);

    ASSERT_NO_THROW(m3 = m3 + m3);
    EXPECT_EQ(m3.get(0,0), 2);
    EXPECT_EQ(m3.get(1,0), 18);
    EXPECT_EQ(m3.get(2,0), 4);
    EXPECT_EQ(m3.get(3,0), 10);
}

TEST_F(MatrixTests, MultiplyOperator) {
    //multiplying matrices that can't be multiplied
    EXPECT_ANY_THROW(m1 * m3);

    ASSERT_NO_THROW(m1 = m1 * m2);
    EXPECT_EQ(m1.get(0,0), 48);
    EXPECT_EQ(m1.get(1,1), 50);
    EXPECT_EQ(m1.get(2,2), 91);
    //result of multiplying 2x4 matrix and 4x2 matrix should be a 2x2 matrix
    Matrix res = Matrix(2,2);
    ASSERT_NO_THROW(res = m4 * m3);
    EXPECT_EQ(res.get(0,0), 51);
    EXPECT_EQ(res.get(0,1), 87);
    EXPECT_EQ(res.get(1,0), 70);
    EXPECT_EQ(res.get(1,1), 96);
    //testing 2x2 format
    EXPECT_ANY_THROW(res.get(2,1));

    //multiplying matrix by a constant value
    //using a 1x1 matrix m5
    ASSERT_TRUE(m5.set(0,0, 5));
    m5 = m5 * 5;
    EXPECT_EQ(m5.get(0,0), 25);
}

TEST_F(MatrixTests, SolveEquation) {
    std::vector<double> res;
    std::vector<double> rightSide(3, 1);
    std::vector<double> rightSideInvalid(2,1);
    std::vector<double> rightSideInvalid2(2,1);

    ASSERT_NO_THROW(res = m1.solveEquation(rightSide));
    EXPECT_EQ(res[0], 0.21);
    EXPECT_EQ(res[1], 0.12);
    EXPECT_EQ(res[2], -0.23);
    //right side has less rows than matrix
    EXPECT_ANY_THROW(m1.solveEquation(rightSideInvalid));
    //m3 isn't square matrix
    EXPECT_ANY_THROW(m3.solveEquation(rightSideInvalid2));
    //singular matrix test
    Matrix singular = Matrix(2,2);
    ASSERT_TRUE(singular.set({{0, 0}, {0, 0}}));
    EXPECT_ANY_THROW(singular.solveEquation(rightSideInvalid));
}

TEST_F(MatrixTests, Determinant) {
    //determinant has many branches, that's why
    //I seperate it from SolveEquation
    Matrix m1x1 = Matrix();
    m1x1.set(0,0,1);
    Matrix m2x2 = Matrix(2,2);
    m2x2.set({{2, 1}, {1, 2}});
    Matrix m4x4 = Matrix(4,4);
    m4x4.set({{2, 1, 1, 1},{1, 3, 5, 1},{8, 1, 5, 1},{1, 0, 1, 3}});
    EXPECT_NO_THROW(m1x1.solveEquation({1}));
    EXPECT_NO_THROW(m2x2.solveEquation({1,1}));
    EXPECT_NO_THROW(m4x4.solveEquation({1,1,1,1}));
}

TEST_F(MatrixTests, Transpose) {
    m1 = m1.transpose();
    EXPECT_EQ(m1.get(0,1), 8);
    EXPECT_EQ(m1.get(2,0), 1);
    EXPECT_EQ(m1.get(1,2), 6);

    //transposing 4x2 matrix, result should be 2x4
    m3 = m3.transpose();
    EXPECT_EQ(m3.get(0,1), 9);
    EXPECT_EQ(m3.get(1,2), 6);
    EXPECT_EQ(m3.get(0,3), 5);
    EXPECT_ANY_THROW(m3.get(2,4));
}

TEST_F(MatrixTests, Inverse) {
    EXPECT_ANY_THROW(m5.inverse());

    m2.set({{1,1,1}, {1,1,1}, {1,1,1}});
    EXPECT_ANY_THROW(m2.inverse());

    ASSERT_NO_THROW(m1 = m1.inverse());
    EXPECT_EQ(m1.get(0,0), 0.1);
    EXPECT_EQ(m1.get(1,1), -0.12);
    EXPECT_EQ(m1.get(2,2), 0.34);

    m5 = Matrix(2,2);
    m5.set({{1,2},{3,4}});
    ASSERT_NO_THROW(m5 = m5.inverse());
    EXPECT_EQ(m5.get(0,0), -2);
    EXPECT_EQ(m5.get(1,1), -0.5);
}


/*** Konec souboru white_box_tests.cpp ***/
