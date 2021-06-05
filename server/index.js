var express = require('express')
var app = express()
var bodyParser = require('body-parser')

require('dotenv').config();

/** IMPORTANT
 * Please ensure that you have installed all the dependencies and the
 * configuration at ".env-example" has been copied and filled into ".env".
*/

// Initialize CouchDB
const nano = require('nano')(`http://${process.env.COUCHDB_USER}:${process.env.COUCHDB_PASSWORD}@${process.env.COUCHDB_HOST}:${process.env.COUCHDB_PORT}`);
const db = nano.db.use(process.env.COUCHDB_DATABASE);

app.get('/', function (req, res) {
  console.log("Hello World")
  res.send("Hello World")
})

app.post('/updateData', bodyParser.json(), function (req, res) {
  console.log(req.body)
  var data = req.body;
  data["_id"] = req.body.deviceId + ":" + Math.floor(new Date().getTime() / 1000)

  db.insert(data).then(function (){
    res.send("Data received")
  })
})

app.listen(3000, function () {
  console.log('Example app listening on port 3000!');
});
