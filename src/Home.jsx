import React, { Component } from 'react'
import { BrowserRouter as Router } from 'react-router-dom'

import {
    MDBJumbotron,
    MDBContainer,
    MDBRow, MDBCol,
    MDBTypography, MDBBox,
    MDBNav, MDBNavItem, MDBNavLink,
    MDBTabContent, MDBTabPane
} from 'mdbreact'

import theme from 'prism-react-renderer/themes/dracula'

import CodeBlock from './CodeBlock'
import Logo128 from './img/logo128.png'

export default class Home extends Component {
    constructor(props) {
        super(props);

        this.state = {
            code_init: `#include <matrixpp/matrix.h>

namespace mpp = matrixpp; // More convenient namespace alias

int main()
{
    auto m = mpp::matrix<int>{};

    return 0;
}`,
            code_showcase_kinds: `#include <matrixpp/matrix.h>

namespace mpp = matrixpp;

int main()
{
    auto m_fully_static = mpp::matrix<int, 3, 3>{};
    auto m_fully_dynamic = mpp::matrix<int, std::dynamic_extent, std::dynamic_extent>{};
    // ^ or just mpp::matrix<int>{} since the size parameters are defaulted to dynamic extent
    auto m_dynamic_rows = mpp::matrix<int, std::dynamic_Extent, 3>{};
    auto m_dynamic_columns = mpp::matrix<int, 3, std::dynamic_extent>{};
    // ^ or just mpp::matrix<int, 3>{} since the column parameter is defaulted to dynamic extent

    return 0;
}`,
            code_showcase_operations: `#include <matrixpp/matrix.h>
#include <matrixpp/operations.hpp> // Required for +, -, *, and /
#include <matrixpp/algorithms.hpp> // Required for determinant, inverse, transpose, block, and etc...

int main()
{
    auto m = mpp::matrix{{
        { 1, 2, 3 },
        { 1, 2, 3 },
        { 1, 2, 3 }
    }}; // Uses 2D initializer list initializer. Defaults to dynamic matrix

    // Remember that math operations are expression templates, so the results are not evaluated immediately
    auto expr = m + m - m * 2 + m / 3;

    auto result = expr(0, 0); // This only evaluates the result at (1, 1)
    auto result_m = mpp::matrix{ expr }; // Evaluates the whole expression and constructs a matrix

    auto m_det = mpp::det(m); // You can specify larger types for safety to avoid type overflow from calculation
    auto m_inv = mpp::inverse<float>(m); // Optional template parameter to allow custom precision types,
                                         // since inverse usually outputs floatings, it's a good candidate
    auto m_transposed = mpp::transpose(m);
    auto m_block = mpp::block(m, 0, 0, 1, 1); // Grabs top corner 2 x 2 (the indexes are inclusive, so 1 x 1
                                              // blocks are possible :) )

    return 0;
}`,
            active_tab: 'fully_dynamic_matrices'
        };

        this.goToTab = this.goToTab.bind(this);
    }

    goToTab(tabName) {
        if (this.state.active_tab != tabName) {
            // If the user is already on the current tab, then there's no need to do anything
            this.setState({ active_tab: tabName });
        }
    }

    render() {
        return (
            <div>
                {/* Home jumbotron */}
                <MDBContainer className='mt-5 text-center'>
                    <MDBRow>
                        <MDBCol>
                            <MDBJumbotron>
                                <img src={Logo128} />
                                <br />
                                <br />
                                <p className='lead'>A header-only C++20 matrix library available for public use under Apache-2.0 License</p>
                                <MDBRow center>
                                    <MDBCol size='2'>
                                        <img src='https://img.shields.io/azure-devops/build/samestimable2016/matrixpp/2/main?label=%F0%9F%94%A8%20Build%20Status' />
                                    </MDBCol>
                                    <MDBCol size='2'>
                                        <img src='https://img.shields.io/azure-devops/tests/samestimable2016/matrixpp/2/main?label=%F0%9F%A7%AA%20Test%20Results' />
                                    </MDBCol>
                                    <MDBCol size='2'>
                                        <img src='https://img.shields.io/azure-devops/coverage/samestimable2016/matrixpp/2/main?label=%F0%9F%93%B6%20Code%20Coverage' />
                                    </MDBCol>
                                    <MDBCol size='2'>
                                        <img src='https://img.shields.io/github/deployments/sam20908/matrixpp/github-pages?label=%F0%9F%9A%80%20GitHub%20Pages' />
                                    </MDBCol>
                                </MDBRow>
                            </MDBJumbotron>
                        </MDBCol>
                    </MDBRow>
                </MDBContainer>
                {/* Overview */}
                <MDBContainer className='mt-5'>
                    <MDBTypography tag='h1' variant='display-4' className='text-center'>Brief Overview</MDBTypography>
                    <hr />
                    <MDBBox tag='p' className='lead'>
                        <b>matrixpp</b> uses free function style algorithms in combination with templates to create an easy-to-understand API for the users.
                        It also currently uses various metaprogramming techniques such as Expression Templates to allow delayed complex matrix computations.
                        It is also compatible with Standard Template Library algorithms.
                    </MDBBox>
                </MDBContainer>
                {/* Examples */}
                <MDBContainer className='mt-5'>
                    <MDBTypography tag='h1' variant='display-4' className='text-center'>Examples</MDBTypography>
                    <hr />
                    <MDBTypography tag='p' className='lead'>To create a matrix, we can simply do this:</MDBTypography>
                    <CodeBlock code={this.state.code_init} theme={theme} />
                    <MDBTypography variant='p' className='lead'>
                        We just created a default initialized <b>fully dynamic</b> matrix with dimensions <b>0 x 0</b>. Fully dynamic means it can resize in
                        rows and columns at runtime. This leads into the 4 kinds of matrices that can be created:
                    </MDBTypography>
                    <Router>
                        <MDBNav className='justify-content-center font-weight-bold'>
                            <MDBNavItem>
                                <MDBNavLink
                                    to='#!'
                                    active={this.state.active_tab === 'fully_dynamic_matrices'}
                                    onClick={() => this.goToTab('fully_dynamic_matrices')}
                                    role='tab'>
                                    Fully Dynamic Matrices
                                    </MDBNavLink>
                            </MDBNavItem>
                            <MDBNavItem>
                                <MDBNavLink
                                    to='#!'
                                    active={this.state.active_tab === 'fully_static_matrices'}
                                    onClick={() => this.goToTab('fully_static_matrices')}
                                    role='tab'>
                                    Fully Static Matrices
                                    </MDBNavLink>
                            </MDBNavItem>
                            <MDBNavItem>
                                <MDBNavLink
                                    to='#!'
                                    active={this.state.active_tab === 'dynamic_row_matrices'}
                                    onClick={() => this.goToTab('dynamic_row_matrices')}
                                    role='tab'>
                                    Dynamic Row Matrices
                                    </MDBNavLink>
                            </MDBNavItem>
                            <MDBNavItem>
                                <MDBNavLink
                                    to='#!'
                                    active={this.state.active_tab === 'dynamic_column_matrices'}
                                    onClick={() => this.goToTab('dynamic_column_matrices')}
                                    role='tab'>
                                    Dynamic Column Matrices
                                    </MDBNavLink>
                            </MDBNavItem>
                        </MDBNav>
                    </Router>
                    <MDBTabContent className='card px-3 pt-2' activeItem={this.state.active_tab}>
                        <MDBTabPane tabId='fully_static_matrices' role='tabpanel'>
                            <MDBTypography tag="p" className="mt-2">
                                A matrix which has fixed dimensions and rows, therefore it cannot be resized. It can be constructed
                                and manipulated at compile time and runtime.
                                    </MDBTypography>
                        </MDBTabPane>
                        <MDBTabPane tabId='fully_dynamic_matrices' role='tabpanel'>
                            <MDBTypography tag="p" className="mt-2">
                                A matrix which can be resized in rows and columns. It can be constructed and manipulated at runtime <b>
                                    (compile time support is pending).</b>
                            </MDBTypography>
                        </MDBTabPane>
                        <MDBTabPane tabId='dynamic_row_matrices' role='tabpanel'>
                            <MDBTypography tag="p" className="mt-2">
                                A matrix which can be resized in rows but has fixed columns. It can be constructed and manipulated at
                                        runtime <b>(compile time support is pending).</b>
                            </MDBTypography>
                        </MDBTabPane>
                        <MDBTabPane tabId='dynamic_column_matrices' role='tabpanel'>
                            <MDBTypography tag="p" className="mt-2">
                                A matrix which can be resized in columns but has fixed rows. It can be constructed and manipulated at
                                        runtime <b>(compile time support is pending).</b>
                            </MDBTypography>
                        </MDBTabPane>
                    </MDBTabContent>
                    <p className="lead mt-5">We can specify the kind of matrix we want by specifying the size extent template parameters:</p>
                    <CodeBlock code={this.state.code_showcase_kinds} theme={theme} />
                    <p className="lead mt-5">Here are some examples of matrix operations:</p>
                    <CodeBlock code={this.state.code_showcase_operations} theme={theme} />
                </MDBContainer>
            </div>
        );
    }
}