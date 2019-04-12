const express = require("express");
const app = express();
const routes = require("./routes");


app.use("/", routes);

app.listen(3000, () => {
    console.log("We've now got a server!");
    console.log("Your routes will be running on http://localhost:3000");
});