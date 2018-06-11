const express = require('express');
const router = express.Router();

router.use('/', require('./chat.js'));

module.exports = router;