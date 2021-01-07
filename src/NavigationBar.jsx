import React, { Component } from 'react'
import { BrowserRouter as Router } from 'react-router-dom'

import {
    MDBNavbar, MDBNavbarBrand, MDBNavbarToggler,
    MDBCollapse,
    MDBNavbarNav, MDBNavItem, MDBNavLink
} from "mdbreact";

import NavbarBrandLogo from './img/logo-navbar-brand.png'

export default class NavigationBar extends Component {
    constructor(props) {
        super(props);

        this.state = { navbar_open: true };
        this.toggleNavbarCollapse = this.toggleNavbarCollapse.bind(this);
    }

    toggleNavbarCollapse() {
        this.setState({ navbar_open: !this.state.navbar_open });
    }

    render() {
        return (
            <Router>
                <MDBNavbar color="black" dark expand="md">
                    <MDBNavbarBrand>
                        <img src={NavbarBrandLogo} alt="" />
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
                                <MDBNavLink to="#!" disabled>A header-only C++20 matrix library</MDBNavLink>
                            </MDBNavItem>
                        </MDBNavbarNav>
                    </MDBCollapse>
                </MDBNavbar>
            </Router>
        );
    }
}