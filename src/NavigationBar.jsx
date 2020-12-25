import React, { Component } from 'react'

import Navbar from 'react-bootstrap/Navbar'
import Nav from 'react-bootstrap/Nav'

export class NavigationBar extends Component {
    render() {
        return (
            <Navbar bg="dark" variant="dark" sticky="top" expand="lg">
                <Navbar.Brand href="#home">matrixpp</Navbar.Brand>
                <Navbar.Toggle aria-controls="basic-navbar-nav" />
                <Navbar.Collapse id="basic-navbar-nav">
                    <Nav className="mr-auto">
                        <Nav.Link href="#home" active="true">Home</Nav.Link>
                        <Nav.Link href="#link">Documentation</Nav.Link>
                    </Nav>
                </Navbar.Collapse>
            </Navbar>
        );
    }
}