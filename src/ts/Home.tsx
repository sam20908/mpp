import * as React from "react";
import { BrowserRouter as Router } from "react-router-dom";

import {
  MDBJumbotron,
  MDBContainer,
  MDBRow,
  MDBCol,
  MDBTypography,
  MDBNav,
  MDBNavItem,
  MDBNavLink,
  MDBTabContent,
  MDBTabPane,
  MDBBtn,
} from "mdbreact";

import theme from "prism-react-renderer/themes/dracula";

import { init, kinds, operations } from "./ExampleCodes";

import CodeBlock from "./CodeBlock";
import Logo128 from "../img/logo128.png";

interface State {
  active_tab: string;
}

export default class Home extends React.Component<unknown, State> {
  state: State = { active_tab: "fully_dynamic_matrices" };

  goToTab(tabName: string): void {
    if (this.state.active_tab != tabName) {
      // If the user is already on the current tab, then there's no need to do anything
      this.setState({ active_tab: tabName });
    }
  }

  render(): React.ReactNode {
    return (
      <div>
        {/* Home jumbotron */}
        <MDBContainer className="mt-5 text-center">
          <MDBRow>
            <MDBCol>
              <MDBJumbotron>
                <img src={Logo128} />
                <br />
                <br />
                <p className="lead">
                  A header-only C++20 matrix library available for public use
                  under Apache-2.0 License
                </p>
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
                <MDBBtn
                  gradient="blue"
                  className="mt-5"
                  href="https://github.com/sam20908/matrixpp"
                >
                  GitHub Repository
                </MDBBtn>
              </MDBJumbotron>
            </MDBCol>
          </MDBRow>
        </MDBContainer>
        {/* Overview */}
        <MDBContainer className="mt-5">
          <MDBTypography tag="h1" variant="display-4" className="text-center">
            Brief Overview
          </MDBTypography>
          <hr />
          <MDBTypography tag="p" className="lead mt-3">
            <b>matrixpp</b> strives to mimic STL-style interface with algorithms
            and operations declared outside of the matrix class to allow users
            to &quot;Pay For What You Use&quot;. The API was also designed to be
            compatible with standard STL algorithms, which means it strives to
            meet the C++ Named Requirements for whatever operation it supports{" "}
            <a href="https://en.cppreference.com/w/cpp/named_req">
              (see all named requirements currently in the Standard)
            </a>
            . It internally uses template metaprogramming to allow techniques
            like <b>expression template objects</b>, which for example delays
            evaluation for a given expression until it needs to be evaluated.
            Lastly, <b>matrixpp</b> is designed to be widely customizable, with
            the help of &quot;Customization Point Objects&quot; to allow
            customizing all algorithms and operations in the library for certain
            user-defined types.
          </MDBTypography>
          <br />
          <MDBTypography blockquote bqColor="primary">
            <MDBTypography tag="p" className="bq-title">
              Notice!
            </MDBTypography>
            <p>
              This project is developed as a hobby and a learning project. I do
              not claim this library to be the &quot;next standard matrix
              library&quot;, but I do try to improve it as time goes on.{" "}
              <b>
                If you want to help contribute, just send an issue or a pull
                request on matrixpp GitHub!
              </b>
            </p>
          </MDBTypography>
        </MDBContainer>
        {/* Examples */}
        <MDBContainer className="mt-5">
          <MDBTypography tag="h1" variant="display-4" className="text-center">
            Examples
          </MDBTypography>
          <hr />
          <MDBTypography tag="p" className="lead">
            To create a matrix, we can simply do this:
          </MDBTypography>
          <CodeBlock code={init} theme={theme} />
          <MDBTypography tag="p" className="lead">
            We just created a default initialized <b>fully dynamic</b> matrix
            with dimensions <b>0 x 0</b>. Fully dynamic means it can resize in
            rows and columns at runtime. This leads into the 4 kinds of matrices
            that can be created:
          </MDBTypography>
          <Router>
            <MDBNav className="justify-content-center font-weight-bold">
              <MDBNavItem>
                <MDBNavLink
                  to="#"
                  active={this.state.active_tab === "fully_dynamic_matrices"}
                  onClick={() => this.goToTab("fully_dynamic_matrices")}
                  role="tab"
                >
                  Fully Dynamic Matrices
                </MDBNavLink>
              </MDBNavItem>
              <MDBNavItem>
                <MDBNavLink
                  to="#"
                  active={this.state.active_tab === "fully_static_matrices"}
                  onClick={() => this.goToTab("fully_static_matrices")}
                  role="tab"
                >
                  Fully Static Matrices
                </MDBNavLink>
              </MDBNavItem>
              <MDBNavItem>
                <MDBNavLink
                  to="#"
                  active={this.state.active_tab === "dynamic_row_matrices"}
                  onClick={() => this.goToTab("dynamic_row_matrices")}
                  role="tab"
                >
                  Dynamic Row Matrices
                </MDBNavLink>
              </MDBNavItem>
              <MDBNavItem>
                <MDBNavLink
                  to="#"
                  active={this.state.active_tab === "dynamic_column_matrices"}
                  onClick={() => this.goToTab("dynamic_column_matrices")}
                  role="tab"
                >
                  Dynamic Column Matrices
                </MDBNavLink>
              </MDBNavItem>
            </MDBNav>
          </Router>
          <MDBTabContent
            className="card px-3 pt-2"
            activeItem={this.state.active_tab}
          >
            <MDBTabPane tabId="fully_static_matrices" role="tabpanel">
              <MDBTypography tag="p" className="mt-2">
                A matrix which has fixed dimensions and rows, therefore it
                cannot be resized. It can be constructed and manipulated at
                compile time and runtime.
              </MDBTypography>
            </MDBTabPane>
            <MDBTabPane tabId="fully_dynamic_matrices" role="tabpanel">
              <MDBTypography tag="p" className="mt-2">
                A matrix which can be resized in rows and columns. It can be
                constructed and manipulated at runtime{" "}
                <b>(compile time support is pending).</b>
              </MDBTypography>
            </MDBTabPane>
            <MDBTabPane tabId="dynamic_row_matrices" role="tabpanel">
              <MDBTypography tag="p" className="mt-2">
                A matrix which can be resized in rows but has fixed columns. It
                can be constructed and manipulated at runtime{" "}
                <b>(compile time support is pending).</b>
              </MDBTypography>
            </MDBTabPane>
            <MDBTabPane tabId="dynamic_column_matrices" role="tabpanel">
              <MDBTypography tag="p" className="mt-2">
                A matrix which can be resized in columns but has fixed rows. It
                can be constructed and manipulated at runtime{" "}
                <b>(compile time support is pending).</b>
              </MDBTypography>
            </MDBTabPane>
          </MDBTabContent>
          <MDBTypography tag="p" className="lead mt-5">
            We can specify the kind of matrix we want by specifying the size
            extent template parameters:
          </MDBTypography>
          <CodeBlock code={kinds} theme={theme} />
          <MDBTypography tag="p" className="lead mt-5">
            Here are some examples of matrix operations:
          </MDBTypography>
          <CodeBlock code={operations} theme={theme} />
          <MDBTypography tag="p" className="lead mt-5">
            Some APIs have overloads that take a custom template type by{" "}
            <b>std::type_identity</b>, which is required because the free
            functions are <b>customization point objects</b>, therefore passing
            a type directly is not possible.
          </MDBTypography>
        </MDBContainer>
      </div>
    );
  }
}
