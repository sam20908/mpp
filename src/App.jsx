import React, { Component } from 'react'

import { Octokit } from '@octokit/rest'

import Button from 'react-bootstrap/Button'
import Jumbotron from 'react-bootstrap/Jumbotron'
import Row from 'react-bootstrap/Row'
import Col from 'react-bootstrap/Col'
import Image from 'react-bootstrap/Image'
import Container from 'react-bootstrap/Container'

import Logo128 from './img/logo128.png'

export class App extends Component {
    constructor(props) {
        super(props);

        this.state = {
            owner: 'sam20908',
            repo: 'matrixpp'
        };

        this.goToRepo = this.goToRepo.bind(this);
        this.goToReleaseURL = this.goToReleaseURL.bind(this);

        this.octokit = new Octokit();
    }

    getLatestRelease() {
        return this.octokit.repos.getLatestRelease({
            owner: this.state.owner,
            repo: this.state.repo
        });
    }

    getRepoURL() {
        return this.octokit.repos.get({
            owner: this.state.owner,
            repo: this.state.repo
        });
    }

    goToRepo() {
        window.open(this.state.repo_url);
    }

    goToReleaseURL() {
        window.open(this.state.release_url);
    }

    componentDidMount() {
        this.getLatestRelease()
            .then(data => {
                this.setState({
                    has_releases: true,
                    latest_version: data.data.tag_name,
                    release_url: data.data.html_url
                });
            })
            .catch(() => {
                // If there is no release, then the awaited promise will be an error, and
                // we have to catch it
                this.setState({
                    has_releases: false
                });
            });

        this.getRepoURL()
            .then(data => {
                this.setState({
                    repo_url: data.data.html_url
                });
            });
    }

    render() {
        let download_button;

        if (this.state.has_releases) {
            download_button = <Button variant="outline-success"
                onClick={() => this.goToReleaseURL()}>Download latest release</Button>;
        }
        else {
            download_button = <Button variant="outline-danger" disabled>No release available</Button>;
        }

        return (
            <div>
                <Jumbotron className="text-center">
                    <Image src={Logo128} />
                    <p>
                        A header-only C++20 matrix library available for public use under Apache-2.0 License
                    </p>
                    <Container>
                        <Row className="justify-content-md-center">
                            <Col md="auto">
                                <img src="https://img.shields.io/azure-devops/build/samestimable2016/matrixpp/2/main?label=%F0%9F%94%A8%20Build%20Status" />
                            </Col>
                            <Col md="auto">
                                <img src="https://img.shields.io/azure-devops/tests/samestimable2016/matrixpp/2/main?label=%F0%9F%A7%AA%20Test%20Results" />
                            </Col>
                            <Col md="auto">
                                <img src="https://img.shields.io/azure-devops/coverage/samestimable2016/matrixpp/2/main?label=%F0%9F%93%B6%20Code%20Coverage" />
                            </Col>
                            <Col md="auto">
                                <img src="https://img.shields.io/github/deployments/sam20908/matrixpp/github-pages?label=%F0%9F%9A%80%20GitHub%20Pages" />
                            </Col>
                        </Row>
                    </Container><br />
                    <Button variant="primary" onClick={() => this.goToRepo()}>GitHub Repository</Button>{' '}
                    {download_button}
                </Jumbotron>
            </div>
        );
    }
}