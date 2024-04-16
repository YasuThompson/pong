#First Chapter with an Example of Pong

![Alt text](images/pong_basic.png?raw=true "Overview")

## Exercise 1.1
The original Pong game supported two players. Instead of the wall on the right side of the screen, I want you to remove it and place a second paddle that player 2 will control. To move this paddle up and down, use the [I] key and the [K] key. The second paddle needs to replicate all the functions of the first paddle. That means you'll need member variables for the paddle's position and direction, code to handle player 2's input, code to draw the paddle, and code to update the paddle. You'll also need to modify the code related to ball collisions to ensure the ball collides correctly with both paddles.

### TODO
- [x] To add multiplayer mode
- [x] Erase the right wall
- [x] Make the right paddle movable
- [x] Make collision detection at the right paddle
- [ ] Final adjustmetns 

## Exercise 1.2
Many pinball games support "multiball," where multiple balls are in play simultaneously. Multiball can be fun in Pong too! To support multiball, let's first create a Ball struct containing two Vector2 variables for position and velocity. Then, add a member variable of type `std::vector<Ball>` to the Game class to accommodate multiple balls. Next, modify the code in `Game::Initialize` to initialize the positions and velocities of multiple balls. In `Game::UpdateGame`, change the ball update code. Instead of using mBallVel and mBallPos variables, loop through the `std::vector` in the code to update all the balls.

### TODO
- [ ] 
- [ ] 


## Additional exercises
- [ ]
- [ ]



### Bug notes
- Hit detection of the ball at the left paddle is not accurate. (A hit is detected a little bit outside the left paddle.)
