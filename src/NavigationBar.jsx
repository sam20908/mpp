import React, { Component } from 'react'

import Navbar from 'react-bootstrap/Navbar'
import Nav from 'react-bootstrap/Nav'
import Container from 'react-bootstrap/Container'

import NavbarBrandLogo from './img/logo-navbar-brand.png'

export class NavigationBar extends Component {
    render() {
        return (
            <Navbar bg="dark" variant="dark" sticky="top" expand="lg">
                <Container fluid>
                    <Navbar.Brand>
                        <img src={NavbarBrandLogo} />
                    </Navbar.Brand>
                    <Navbar.Toggle aria-controls="responsive-navbar-nav" />
                    <Navbar.Collapse id="responsive-navbar-nav">
                        <Nav className="mr-auto" activeKey="/">
                            <Nav.Link href="/">Home</Nav.Link>
                            <Nav.Link href="/docs">Documentation</Nav.Link>
                        </Nav>
                        <Navbar.Text>
                            A header-only C++20 matrix library
                    </Navbar.Text>
                    </Navbar.Collapse>
                </Container>
            </Navbar>
        );
    }
}