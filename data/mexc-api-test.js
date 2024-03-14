const axios = require('axios');

// Base URL of the API
const baseURL = 'https://api.mexc.com';

// Function to test the /api/v3/defaultSymbols endpoint
function testDefaultSymbols() {
    axios.get(`${baseURL}/api/v3/defaultSymbols`)
        .then(response => {
            console.log('Default Symbols:', response.data);
        })
        .catch(error => {
            console.error('Error:', error);
        });
}

// Function to test the /api/v3/exchangeInfo endpoint
function testExchangeInfo() {
    axios.get(`${baseURL}/api/v3/exchangeInfo`)
        .then(response => {
            console.log('Exchange Info:', response.data);
        })
        .catch(error => {
            console.error('Error:', error);
        });
}

// Run the tests
testDefaultSymbols();
testExchangeInfo();