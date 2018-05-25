const express = require('express');
const router = express.Router();

router.use('/signup', require('./signup.js'));	// ip:3000/signup
router.use('/signin', require('./signin.js'));	// ip:3000/signin

router.use('/', require('./board/index.js'));		// ip:3000/
module.exports = router;

//---------------------------------------------LSM's Index
router.use('/chat', require('./chat/index.js');