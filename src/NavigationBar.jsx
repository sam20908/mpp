import React, { Component } from 'react'

import Navbar from 'react-bootstrap/Navbar'
import Nav from 'react-bootstrap/Nav'

export class NavigationBar extends Component {
    render() {
        return (
            <Navbar bg="dark" variant="dark" sticky="top" expand="lg">
                <Navbar.Brand href="#home">matrixpp</Navbar.Brand>
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
            </Navbar>
        );
    }
}