const express = require("express");
const router = express.Router();

router.get("/", (req, res) => {
    res.sendFile(`${__dirname}/views/main.html`);
});

router.get("/refer", (req, res) => {
    res.sendFile(`${__dirname}/views/cs.html`);
});

router.get("/personalInfo", (req, res) => {
    res.sendFile(`${__dirname}/views/personalinfo.html`);
});

router.get("/solution", (req, res) => {
    res.sendFile(`${__dirname}/views/solution.html`);
});
router.get("/homework", (req, res) => {
    res.sendFile(`${__dirname}/views/homework.html`);
});

module.exports = router;