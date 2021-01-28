import "@fortawesome/fontawesome-free/css/all.min.css";
import "bootstrap-css-only/css/bootstrap.min.css";
import "mdbreact/dist/css/mdb.css";

import * as React from "react";
import * as ReactDOM from "react-dom";

import NavigationBar from "./ts/NavigationBar";
import Home from "./ts/Home";

ReactDOM.render(
	<React.StrictMode>
		<NavigationBar />
		<Home />
	</React.StrictMode>,
	document.getElementById("root")
);
