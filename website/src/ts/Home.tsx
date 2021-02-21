import * as React from "react";

import {
	MDBJumbotron,
	MDBContainer,
	MDBRow,
	MDBCol,
	MDBTypography,
	MDBBtn,
	MDBCard,
	MDBCardTitle,
	MDBCardBody,
	MDBCardText,
} from "mdbreact";

import theme from "prism-react-renderer/themes/dracula";

import {
	kinds,
	operations,
	customize_default_extent,
	customize_utilities,
	custom_iterators,
} from "./ExampleCodes";

import CodeBlock from "./CodeBlock";
import Logo128 from "../img/logo128.png";

export default class Home extends React.Component<unknown, unknown> {
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
									A header-only C++20 matrix library available
									for public use under Apache-2.0 License
								</p>
								<MDBRow center>
									<MDBCol size="2">
										<img src="https://img.shields.io/azure-devops/build/samestimable2016/mpp/3/main?label=%F0%9F%94%A8%20Build%20Status" />
									</MDBCol>
									<MDBCol size="2">
										<img src="https://img.shields.io/azure-devops/tests/samestimable2016/mpp/3/main?label=%F0%9F%A7%AA%20Test%20Results" />
									</MDBCol>
									<MDBCol size="2">
										<img src="https://img.shields.io/azure-devops/coverage/samestimable2016/mpp/3/main?label=%F0%9F%93%B6%20Code%20Coverage" />
									</MDBCol>
									<MDBCol size="2">
										<img src="https://img.shields.io/github/deployments/sam20908/mpp/github-pages?label=%F0%9F%9A%80%20GitHub%20Pages" />
									</MDBCol>
								</MDBRow>
								<MDBBtn
									gradient="blue"
									className="mt-5"
									href="https://github.com/sam20908/mpp"
								>
									GitHub Repository
								</MDBBtn>
							</MDBJumbotron>
						</MDBCol>
					</MDBRow>
				</MDBContainer>
				{/* Overview */}
				<MDBContainer className="mt-5">
					<MDBTypography
						tag="h1"
						variant="display-4"
						className="text-center"
					>
						Brief Overview
					</MDBTypography>
					<hr />
					<MDBTypography tag="p" className="lead mt-3">
						<b>mpp</b> strives to mimic STL-style interface with
						algorithms and operations declared outside of the matrix
						class to allow users to &quot;Pay For What You
						Use&quot;. The API was also designed to be compatible
						with standard STL algorithms, which means it meets the
						C++ Named Requirements for whatever operation it
						supports{" "}
						<a href="https://en.cppreference.com/w/cpp/named_req">
							(see all named requirements currently in the
							Standard)
						</a>
						. It internally uses template metaprogramming to allow
						techniques like <b>expression template objects</b>,
						which for example delays evaluation for a given
						expression until it needs to be evaluated. Lastly,{" "}
						<b>mpp</b> is designed to be widely customizable, with
						the help of <b>customization point objects</b> to allow
						customizing all algorithms and operations in the library
						for certain user-defined types.
					</MDBTypography>
					<br />
					<MDBTypography
						note
						noteColor="secondary"
						noteTitle="Note! "
					>
						This project is developed as a hobby and a learning
						project. I do not claim this library to be the
						&quot;next standard matrix library&quot;, but I do try
						to improve it as time goes on.{" "}
						<b>
							If you want to help contribute, just send an issue
							or a pull request on mpp GitHub!
						</b>
					</MDBTypography>
				</MDBContainer>
				{/* Examples */}
				<MDBContainer className="mt-5">
					<MDBTypography
						tag="h1"
						variant="display-4"
						className="text-center"
					>
						Examples
					</MDBTypography>
					<hr />
					<MDBTypography tag="p" className="lead">
						Below is a sample of creating different kinds of
						matrices
					</MDBTypography>
					<CodeBlock code={kinds} theme={theme} />
					<MDBTypography tag="p" className="lead">
						Below describes the pros and cons of each matrix:
					</MDBTypography>
					<MDBRow className="mb-4">
						<MDBCol sm="6">
							<MDBCard>
								<MDBCardBody>
									<MDBCardTitle>
										Fully Static Matrices
									</MDBCardTitle>
									<MDBCardText>
										A matrix that uses a static buffer{" "}
										<b>std::array</b>, which also means it
										will be allocated on the stack.
										<br />
										<br />
										<b>Pros</b>
										<MDBTypography listUnStyled>
											<ul>
												<li>
													Static buffer (no heap
													allocation)
												</li>
												<li>
													Dimensions known at compile
													time
												</li>
											</ul>
										</MDBTypography>
										<b>Cons</b>
										<MDBTypography listUnStyled>
											<ul>
												<li>Not resizable</li>
											</ul>
										</MDBTypography>
									</MDBCardText>
								</MDBCardBody>
							</MDBCard>
						</MDBCol>
						<MDBCol sm="6">
							<MDBCard>
								<MDBCardBody>
									<MDBCardTitle>
										Fully Dynamic Matrices
									</MDBCardTitle>
									<MDBCardText>
										A matrix that can have a runtime size
										and can be very flexible. It can be
										imagined as <b>std::vector</b> of
										matrices because of how dynamic it can
										be.
										<br />
										<br />
										<b>Pros</b>
										<MDBTypography listUnStyled>
											<ul>
												<li>
													Very flexible (can be
													resized in rows and columns)
												</li>
												<li>
													Default kind of matrix which
													works for almost every
													situation
												</li>
											</ul>
										</MDBTypography>
										<b>Cons</b>
										<MDBTypography listUnStyled>
											<ul>
												<li>
													Size is not known at compile
													time
												</li>
											</ul>
										</MDBTypography>
									</MDBCardText>
								</MDBCardBody>
							</MDBCard>
						</MDBCol>
					</MDBRow>
					<MDBRow className="mb-4">
						<MDBCol sm="6">
							<MDBCard>
								<MDBCardBody>
									<MDBCardTitle>
										Dynamic Rows Matrices
									</MDBCardTitle>
									<MDBCardText>
										A matrix that has known column size but
										can be flexible in rows
										<br />
										<br />
										<b>Pros</b>
										<MDBTypography listUnStyled>
											<ul>
												<li>Resizable in rows</li>
												<li>
													Columns are known at compile
													time
												</li>
											</ul>
										</MDBTypography>
										<b>Cons</b>
										<MDBTypography listUnStyled>
											<ul>
												<li>
													Rows is not known at compile
													time
												</li>
											</ul>
										</MDBTypography>
									</MDBCardText>
								</MDBCardBody>
							</MDBCard>
						</MDBCol>
						<MDBCol sm="6">
							<MDBCard>
								<MDBCardBody>
									<MDBCardTitle>
										Dynamic Columns Matrices
									</MDBCardTitle>
									<MDBCardText>
										A matrix that has known row size but can
										be flexible in columns
										<br />
										<br />
										<b>Pros</b>
										<MDBTypography listUnStyled>
											<ul>
												<li>Resizable in columns</li>
												<li>
													Rows are known at compile
													time
												</li>
											</ul>
										</MDBTypography>
										<b>Cons</b>
										<MDBTypography listUnStyled>
											<ul>
												<li>
													Columns is not known at
													compile time
												</li>
											</ul>
										</MDBTypography>
									</MDBCardText>
								</MDBCardBody>
							</MDBCard>
						</MDBCol>
					</MDBRow>
					<MDBTypography tag="p" className="lead mt-5">
						Here are some examples of matrix operations:
					</MDBTypography>
					<CodeBlock code={operations} theme={theme} />
					<MDBTypography tag="p" className="lead mt-5">
						Some algorithms and operations have overloads that take
						a custom template type by <b>std::type_identity</b>,
						which is required because the algorithms and operations
						are <b>customization point objects</b>, therefore
						passing a type directly to the invoke operator is not
						possible. Currently, the algorithms and operations that
						support custom passing types can deduce the type passed
						in from <b>std::type_identity</b>.
					</MDBTypography>
					<MDBTypography tag="p" className="lead mt-5">
						Here is how you can customize the default extents that
						the matrix will default to:
					</MDBTypography>
					<CodeBlock code={customize_default_extent} theme={theme} />
					<MDBTypography tag="p" className="lead mt-5">
						All the customizations take place in the function{" "}
						<b>tag_invoke</b>. It allows customization of all{" "}
						<b>mpp customization points</b> in one function name,
						which reduces the chance of someone being confused. Here
						is another example with another utility function:
					</MDBTypography>
					<CodeBlock code={customize_utilities} theme={theme} />
					<MDBTypography tag="p" className="lead mt-5">
						mpp also uses custom iterators that meets the Standard
						iterator requirements <b>(LegacyContiguousIterator)</b>{" "}
						with extra functionalities to make nagivating through
						matrices easier.
					</MDBTypography>
					<CodeBlock code={custom_iterators} theme={theme} />
				</MDBContainer>
			</div>
		);
	}
}
