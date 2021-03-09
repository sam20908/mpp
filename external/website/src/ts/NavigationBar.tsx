/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at

 *   http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

import * as React from "react";
import { BrowserRouter as Router } from "react-router-dom";

import {
  MDBNavbar,
  MDBNavbarBrand,
  MDBNavbarToggler,
  MDBCollapse,
  MDBNavbarNav,
  MDBNavItem,
  MDBNavLink,
} from "mdbreact";

import NavbarBrandLogo from "../img/logo-navbar-brand.png";

interface State {
  navbar_open: boolean;
}

export default class NavigationBar extends React.Component<unknown, State> {
  state: State = {
    navbar_open: true,
  };

  toggleNavbarCollapse(): void {
    this.setState({ navbar_open: !this.state.navbar_open });
  }

  render(): React.ReactNode {
    return (
      <Router>
        <MDBNavbar color="black" dark expand="md">
          <MDBNavbarBrand>
            <img src={NavbarBrandLogo} />
          </MDBNavbarBrand>
          <MDBNavbarToggler onClick={this.toggleNavbarCollapse} />
          <MDBCollapse isOpen={this.state.navbar_open} navbar>
            <MDBNavbarNav left>
              <MDBNavItem>
                <MDBNavLink to="#!">Home</MDBNavLink>
              </MDBNavItem>
              <MDBNavItem>
                <MDBNavLink to="#!">Documentation</MDBNavLink>
              </MDBNavItem>
            </MDBNavbarNav>
            <MDBNavbarNav right>
              <MDBNavItem>
                <MDBNavLink to="#!" disabled>
                  A header-only C++20 matrix library
                </MDBNavLink>
              </MDBNavItem>
            </MDBNavbarNav>
          </MDBCollapse>
        </MDBNavbar>
      </Router>
    );
  }
}
