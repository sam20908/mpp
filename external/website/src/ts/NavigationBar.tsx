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
