import React from 'react';
import ReactDOM from 'react-dom';

import { NavigationBar } from "./NavigationBar"

import 'bootstrap/dist/css/bootstrap.min.css';

ReactDOM.render(
  <React.StrictMode>
    <NavigationBar />
  </React.StrictMode>,
  document.getElementById('root')
);
