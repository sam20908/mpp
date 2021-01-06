import '@fortawesome/fontawesome-free/css/all.min.css';
import 'bootstrap-css-only/css/bootstrap.min.css';
import 'mdbreact/dist/css/mdb.css';

import React from 'react';
import ReactDOM from 'react-dom';

import { App } from './App'
import { NavigationBar } from './NavigationBar'

ReactDOM.render(
  <React.StrictMode>
    <NavigationBar />
    <App />
  </React.StrictMode>,
  document.getElementById('root')
);