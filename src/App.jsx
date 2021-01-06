import React, { Component } from 'react'

import Button from 'react-bootstrap/Button'
import Jumbotron from 'react-bootstrap/Jumbotron'

import { Octokit } from '@octokit/rest'
import { throttling } from '@octokit/plugin-throttling'

export class App extends Component {
    constructor(props) {
        super(props);

        this.state = {
            owner: 'sam20908',
            repo: 'matrixpp'
        };

        this.goToRepo = this.goToRepo.bind(this);
        this.goToReleaseURL = this.goToReleaseURL.bind(this);

        const throttledKit = Octokit.plugin(throttling);
        this.octokit = new throttledKit({
            throttle: {
                onRateLimit: (retryAfter, options, octokit) => {
                    octokit.log.error("GitHub API request for latest release is exhausted! Retrying");

                    if (options.request.retryCount === 0) {
                        octokit.log.info(`Retrying after ${retryAfter} seconds!`);
                        return true;
                    }
                },
                onAbuseLimit: (retryAfter, options, octokit) => {
                    octokit.log.warn("GitHub API request abused!");
                }
            },
            auth: "f4b3cbf37e9937a69e7e8bfa2f47f7e5869317a3"
        });
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
        return (
            <div>
                <Jumbotron className="text-center">
                    <h1>matrixpp</h1>
                    <p>
                        A header-only C++20 matrix library available for public use under Apache-2.0 License
                    </p>
                    {/* <img src="https://img.shields.io/azure-devops/build/samestimable2016/matrixpp/2/main?label=%F0%9F%94%A8%20Build%20Status" />
                    <img src="https://img.shields.io/azure-devops/tests/samestimable2016/matrixpp/2/main?label=%F0%9F%A7%AA%20Test%20Results" />
                    <img src="https://img.shields.io/azure-devops/coverage/samestimable2016/matrixpp/2/main?label=%F0%9F%93%B6%20Code%20Coverage" />
                    <img src="https://img.shields.io/github/deployments/sam20908/matrixpp/github-pages?label=%F0%9F%9A%80%20GitHub%20Pages" /> */}
                    <Button variant="primary" onClick={() => this.goToRepo()}>GitHub Repository</Button>{' '}
                    <Button variant="outline-success"
                        onClick={() => this.goToReleaseURL()}
                        disabled={!this.state.has_releases}>Download latest release</Button>{' '}
                </Jumbotron>
            </div>
        );
    }
}