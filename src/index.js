import '@fortawesome/fontawesome-free/css/all.min.css';
import 'bootstrap-css-only/css/bootstrap.min.css';
import 'mdbreact/dist/css/mdb.css';

import React from 'react';
import ReactDOM from 'react-dom';

import NavigationBar from './NavigationBar'
import Home from './Home'

ReactDOM.render(
  <React.StrictMode>
    <NavigationBar />
    <Home />
  </React.StrictMode>,
  document.getElementById('root')
);