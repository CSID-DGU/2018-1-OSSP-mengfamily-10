const express = require('express');
const app = express();
const router = express.Router();

const db = require('../../module/pool.js');

var server = require('http').createServer(app);
var root_io = require('socket.io')(server);


// // usernames which are currently connected to the chat
// var usernames = {};

// // rooms which are currently available in chat
// // var rooms = ['room1','room2','room3'];
// let room_cnt = 1;
// var rooms = [];

// Routing
app.get('/', function (req, res) {
	console.log(' here in chat.js');
	res.sendFile(path.resolve(__dirname + '/../../views/index.html'));
});


root_io.sockets.on('connection', function (socket) {

	// when the client emits 'adduser', this listens and executes
	// user = {u_idx, chatroom_idx};
	socket.on('adduser', async function(user){


		let getUserInfoQuery = 'SELECT * FROM tkb.user WHERE u_idx = ?';
		let getUserInfo = await db.queryParamCnt_Arr(getUserInfoQuery, [user.u_idx]);

		let getChatroomInfoQuery = 'SELECT * FROM tkb.group_chatroom WHERE chatroom_idx = ?';
		let getChatroomInfo = await db.queryParamCnt_Arr(getChatroomInfoQuery, [user.chatroom_idx]);

		// store the username in the socket session for this client
		// socket.username = username;
		socket.username = getUserInfo[0].name;
		socket.room = getChatroomInfo[0].ctrl_name;

		
		console.log('socket room : ' + socket.room);
		console.log('user name : ' + getUserInfo[0]);

		// store the room name in the socket session for this client
		// socket.room = rooms[0];

        // console.log('Client logged-in:\n name:' + socket.username + '\n room: ' + socket.room);

    //     /*
    //     이 부분은 신기하다.
    //     */
    //     // add the client's username to the global list
    //     usernames[socket.username] = socket.username;
				// console.log(usernames);
		// send client to room 1
		socket.join(socket.room);
		// echo to client they've connected
		// socket.emit('updatechat', 'SERVER', 'you have connected to ' + socket.room);
		// echo to room 1 that a person has connected to their room
		// socket.broadcast.to(socket.room).emit('updatechat', 'SERVER', socket.username + ' has connected to this room');



		socket.emit('updaterooms', socket.room);
	});

	// when the client emits 'sendchat', this listens and executes
	socket.on('sendchat', async function (data) {
		// we tell the client to execute 'updatechat' with 2 parameters
		root_io.sockets.in(socket.room).emit('updatechat', socket.username, data);

		let selectQuery = 'SELECT u_idx FROM tkb.user WHERE name = ?';
		let selectResult = await db.queryParamCnt_Arr(selectQuery, [socket.username]);
		let insertQuery = 'INSERT INTO chat.' + socket.room + ' (u_idx, chatroom_idx, data) VALUES (?, ?, ?)';
		console.log("here check");
		console.log(socket.username, socket.room, data);

		let insertResult = await db.queryParamCnt_Arr(insertQuery, [socket.username, socket.room , data]);
		console.log(insertResult);
	});

	// socket.on('switchRoom', function(newroom){
	// 	// leave the current room (stored in session)
	// 	socket.leave(socket.room);
	// 	// join new room, received as function parameter
	// 	socket.join(newroom);
	// 	socket.emit('updatechat', 'SERVER', 'you have connected to '+ newroom);
	// 	// sent message to OLD room
	// 	socket.broadcast.to(socket.room).emit('updatechat', 'SERVER', socket.username+' has left this room');
	// 	// update socket session room title
	// 	socket.room = newroom;
	// 	socket.broadcast.to(newroom).emit('updatechat', 'SERVER', socket.username+' has joined this room');
	// 	socket.emit('updaterooms', rooms, newroom);
	// });

	// when the user disconnects.. perform this
	socket.on('disconnect', function(){
		// remove the username from global usernames list
		// delete usernames[socket.username];
		// console.log(usernames);
		// update list of users in chat, client-side
		root_io.sockets.emit('updateusers', usernames);
		// echo globally that this client has left
		// socket.broadcast.emit('updatechat', 'SERVER', socket.username + ' has disconnected');
		socket.leave(socket.room);
	});

	// // when the client emits 'create_room', this listens and executes
	// socket.on('create_room', function () {
	// 	room_cnt ++; // 나중에 이 부분을 Client가 보내줄 채팅방 이름으로 바꿔야 함.
	// 	rooms.push("room"+room_cnt);
	// 	root_io.emit('updaterooms', rooms, socket.room);
	// });


	// // when the client emits 'delete_room', this listens and executes
	// socket.on('delete_room', function (now_room) {
	// 	room_cnt --;
	// 	let idx = rooms.indexOf(now_room);
	// 	rooms.splice(idx,1);
	// 	root_io.emit('updaterooms', rooms, socket.room);
	// });

});


server.listen(3090, function() {
  console.log('Socket IO server listening on port 3090 in chat.js');
});




module.exports = router;