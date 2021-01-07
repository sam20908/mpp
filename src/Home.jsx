import React, { Component } from 'react'

import { MDBJumbotron, MDBContainer, MDBRow, MDBCol } from 'mdbreact'

// import theme from 'prism-react-renderer/themes/dracula'

// import CodeBlock from './CodeBlock'
import Logo128 from './img/logo128.png'

export default class Home extends Component {
    constructor(props) {
        super(props);

        this.state = {
            code_init: `
            #include <matrixpp/matrix.h>

            namespace mpp = matrixpp; // More convenient namespace alias

            int main()
            {
                auto m = mpp::matrix<int>{};
            }
            `,
            code_showcase_matrix_kinds: `
            #include <matrixpp/matrix.h>

            namespace mpp = matrixpp;

            int main()
            {
                auto m_fully_static = mpp::matrix<int, 3, 3>{};
                auto m_fully_dynamic = mpp::matrix<int, std::dynamic_extent, std::dynamic_extent>{};
                // ^ or just mpp::matrix<int>{} since the size parameters are defaulted to dynamic extent
                auto m_dynamic_rows = mpp::matrix<int, std::dynamic_Extent, 3>{};
                auto m_dynamic_columns = mpp::matrix<int, 3, std::dynamic_extent>{};
                // ^ or just mpp::matrix<int, 3>{} since the column parameter is defaulted to dynamic extent
            }
            `
        };
    }

    render() {
        return (
            <MDBContainer className="mt-5 text-center">
                <MDBRow>
                    <MDBCol>
                        <MDBJumbotron>
                            <img src={Logo128} />
                            <br />
                            <br />
                            <p className="lead">A header-only C++20 matrix library available for public use under Apache-2.0 License</p>
                            <MDBRow center>
                                <MDBCol size="2">
                                    <img src="https://img.shields.io/azure-devops/build/samestimable2016/matrixpp/2/main?label=%F0%9F%94%A8%20Build%20Status" />
                                </MDBCol>
                                <MDBCol size="2">
                                    <img src="https://img.shields.io/azure-devops/tests/samestimable2016/matrixpp/2/main?label=%F0%9F%A7%AA%20Test%20Results" />
                                </MDBCol>
                                <MDBCol size="2">
                                    <img src="https://img.shields.io/azure-devops/coverage/samestimable2016/matrixpp/2/main?label=%F0%9F%93%B6%20Code%20Coverage" />
                                </MDBCol>
                                <MDBCol size="2">
                                    <img src="https://img.shields.io/github/deployments/sam20908/matrixpp/github-pages?label=%F0%9F%9A%80%20GitHub%20Pages" />
                                </MDBCol>
                            </MDBRow>
                        </MDBJumbotron>
                    </MDBCol>
                </MDBRow>
                {/* <br />
                    <Container>
                        <h1 className="display-4 text-center">Brief Overview</h1>
                        <br />
                        <p className="lead">
                            <b>matrixpp</b> uses free function style algorithms in combination with templates to create an easy-to-understand API for the users.
                                It also currently uses various metaprogramming techniques such as Expression Templates to allow fast <b>(working on getting several
                                things faster)</b> complex matrix computations. It is also compatible with Standard Template Library algorithms.
                            </p>
                        <br />
                        <h1 className="display-4 text-center">Example Usage</h1>
                        <br />
                        <p className="lead">To create a matrix, we can simply call the constructor like this:</p>
                        <CodeBlock code={this.state.code_init} theme={theme} />
                        <p className="lead">
                            We just created a <b>fully dynamic</b> matrix, meaning it can resize in rows and columns at runtime. This leads into the 4 kinds of
                                matrices that can be created:
                        </p>
                        <Tab.Container defaultActiveKey="#matrix-kind-fully-dynamic">
                            <Row>
                                <Col>
                                    <ListGroup>
                                        <ListGroup.Item action href="#matrix-kind-fully-dynamic">
                                            Fully dynamic matrices
                                        </ListGroup.Item>
                                        <ListGroup.Item action href="#matrix-kind-fully-static">
                                            Fully static matrices
                                        </ListGroup.Item>
                                        <ListGroup.Item action href="#matrix-kind-dynamic-row">
                                            Dynamic row matrices
                                        </ListGroup.Item>
                                        <ListGroup.Item action href="#matrix-kind-dynamic-column">
                                            Dynamic column matrices
                                        </ListGroup.Item>
                                    </ListGroup>
                                </Col>
                                <Col>
                                    <Tab.Content>
                                        <Tab.Pane eventKey="#matrix-kind-fully-dynamic">
                                            2D matrix which can resize in rows and columns, and it can be manipulated at runtime.
                                        </Tab.Pane>
                                        <Tab.Pane eventKey="#matrix-kind-fully-static">
                                            2D matrix which has a fixed dimension (therefore cannot be resized), and it can be manipulated
                                            at compile time and runtime.
                                        </Tab.Pane>
                                        <Tab.Pane eventKey="#matrix-kind-dynamic-row">
                                            2D matrix which can resize in rows but has fixed columns, and it can be manipulated at runtime.
                                        </Tab.Pane>
                                        <Tab.Pane eventKey="#matrix-kind-dynamic-column">
                                            2D matrix which can resize in columns but has fixed rows, and it can be manipulated at runtime.
                                        </Tab.Pane>
                                    </Tab.Content>
                                </Col>
                            </Row>
                        </Tab.Container>
                        <br />
                        <p className="lead">We can specify the kind of matrix we want by specifying the size extent template parameters:</p>
                        <CodeBlock code={this.state.code_showcase_matrix_kinds} theme={theme} />
                        <p className="lead">
                            <b>One important thing to remember: </b>fully static matrices have <code></code>
                        </p>
                    </Container> */}
            </MDBContainer>
        );
    }
}