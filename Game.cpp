// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;
const float paddle_vel = 300.0f;
float window_height = 1024.0f;
float window_width = 768.0f;

Game::Game(bool multiplePlayerMode)
:mWindow(nullptr)
,mRenderer(nullptr)
,mTicksCount(0)
,mIsRunning(true)
,left_mPaddleDir(0)
,right_mPaddleDir(0)
,mMultiplePlayerMode(multiplePlayerMode)
{
	
}

bool Game::Initialize()
{
	// Initialize SDL
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
    
    
	// Create an SDL Window
	mWindow = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 1)", // Window title
		100,	// Top left x-coordinate of window
		100,	// Top left y-coordinate of window
        (int)window_height,	// Width of window
        (int)window_width,	// Height of window
		0		// Flags (0 for no flags set)
	);

	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	
	//// Create SDL renderer
	mRenderer = SDL_CreateRenderer(
		mWindow, // Window to create renderer for
		-1,		 // Usually -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}
    
	left_mPaddlePos.x = 0.0f;
	left_mPaddlePos.y = window_width/2.0f;
    
    right_mPaddlePos.x = window_height - thickness;
    right_mPaddlePos.y = window_width/2.0f;
    
	mBallPos.x = window_height/2.0f;
	mBallPos.y = window_width/2.0f;
    // TODO: to set this initial velocity randomly
	mBallVel.x = -200.0f;
	mBallVel.y = 235.0f;
    
    
	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			// If we get an SDL_QUIT event, end loop
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}
	
	// Get state of keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// If escape is pressed, also end loop
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}
	
	// Update left paddle direction based on W/S keys
	left_mPaddleDir = 0;
	if (state[SDL_SCANCODE_W])
	{
        left_mPaddleDir -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
        left_mPaddleDir += 1;
	}
    
    // Update right paddle direction based on W/S keys
    right_mPaddleDir = 0;
    if (state[SDL_SCANCODE_I])
    {
        right_mPaddleDir -= 1;
    }
    if (state[SDL_SCANCODE_K])
    {
        right_mPaddleDir += 1;
    }
    
    
}

void Game::UpdateGame()
{
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	// Delta time is the difference in ticks from last frame
	// (converted to seconds)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	
	// Clamp maximum delta time value
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks();
	
	// Update left paddle position based on direction
	if (left_mPaddleDir != 0)
	{
		left_mPaddlePos.y += left_mPaddleDir * paddle_vel * deltaTime;
        
		// Make sure paddle doesn't move off screen when the left paddle is too below.
		if (left_mPaddlePos.y < (paddleH/2.0f + thickness))
		{
			left_mPaddlePos.y = paddleH/2.0f + thickness;
		}
        // Make sure paddle doesn't move off screen when the left paddle is too above.
		else if (left_mPaddlePos.y > (window_width - paddleH/2.0f - thickness))
		{
			left_mPaddlePos.y = window_width - paddleH/2.0f - thickness;
		}
	}
	
    // Update right paddle position based on direction
    if (right_mPaddleDir != 0)
    {
        right_mPaddlePos.y += right_mPaddleDir * paddle_vel * deltaTime;
        
        // Make sure paddle doesn't move off screen when the right paddle is too below.
        if (right_mPaddlePos.y < (paddleH/2.0f + thickness))
        {
            right_mPaddlePos.y = paddleH/2.0f + thickness;
        }
        
        // Make sure paddle doesn't move off screen when the right paddle is too above.
        else if (right_mPaddlePos.y > (window_width - paddleH/2.0f - thickness))
        {
            right_mPaddlePos.y = window_width - paddleH/2.0f - thickness;
        }
        
    }
    
    
	// Update ball position based on ball velocity
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;
    
    
    //TODO: the processes below can be skipped more quickly in most cases
	
	// Bounce if needed
	// Did we intersect with the paddle?
	float left_y_diff = left_mPaddlePos.y - mBallPos.y;
    float right_y_diff = right_mPaddlePos.y - mBallPos.y;
    
    
    // Take absolute value of difference at the left side
    left_y_diff = (left_y_diff > 0.0f) ? left_y_diff : -left_y_diff;
    
    // When the ball collides with the left paddle
	if (
		// Our y-difference is small enough
        left_y_diff <= paddleH / 2.0f &&
		// We are in the correct x-position
        // TODO: to write this paddle-ball collision rule in a better way
        20.0f <= mBallPos.x && mBallPos.x <= 25.0f &&
		// The ball is moving to the left
		mBallVel.x < 0.0f)
	{
        // The ball bounces back
		mBallVel.x *= -1.0f;
	}
    
	// Did the ball go off the screen? (if so, end game)
	else if (mBallPos.x <= 0.0f)
	{
        // Game over
		mIsRunning = false;
	}
    
    
    
	// Did the ball collide with the right wall?
	else if (mBallPos.x >= (window_height - thickness) && mBallVel.x > 0.0f)
	{
		mBallVel.x *= -1.0f;
	}
	
	// Did the ball collide with the top wall?
	if (mBallPos.y <= thickness && mBallVel.y < 0.0f)
	{
		mBallVel.y *= -1;
	}
	// Did the ball collide with the bottom wall?
	else if (mBallPos.y >= (window_width - thickness) &&
		mBallVel.y > 0.0f)
	{
		mBallVel.y *= -1;
	}
    
    
    // Take absolute value of difference at the right side
    right_y_diff = (right_y_diff > 0.0f) ? right_y_diff : -right_y_diff;
    if (
        // Our y-difference is small enough
        right_y_diff <= paddleH / 2.0f &&
        // We are in the correct x-position
        20.0f <= mBallPos.x && mBallPos.x <= 25.0f &&
        // The ball is moving to the left
        mBallVel.x < 0.0f)
    {
        mBallVel.x *= -1.0f;
    }
    // Did the ball go off the screen? (if so, end game)
    else if (mBallPos.x <= 0.0f)
    {
        mIsRunning = false;
    }
    // Did the ball collide with the right wall?
    else if (mBallPos.x >= (window_height - thickness) && mBallVel.x > 0.0f)
    {
        mBallVel.x *= -1.0f;
    }
    
    // Did the ball collide with the top wall?
    if (mBallPos.y <= thickness && mBallVel.y < 0.0f)
    {
        mBallVel.y *= -1;
    }
    // Did the ball collide with the bottom wall?
    else if (mBallPos.y >= (window_width - thickness) &&
        mBallVel.y > 0.0f)
    {
        mBallVel.y *= -1;
    }
    
    
    
    
}

void Game::GenerateOutput()
{
	// Set draw color to blue
	SDL_SetRenderDrawColor(
		mRenderer,
		0,		// R
		0,		// G 
		255,	// B
		255		// A
	);
	
	// Clear back buffer
	SDL_RenderClear(mRenderer);

	// Draw walls
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	
	// Draw top wall
	SDL_Rect wall{
		0,			// Top left x
		0,			// Top left y
		int(window_height),		// Width
		thickness	// Height
	};
	SDL_RenderFillRect(mRenderer, &wall);
	
	// Draw bottom wall
	wall.y = int(window_width) - thickness;
	SDL_RenderFillRect(mRenderer, &wall);
	
    if(mMultiplePlayerMode==false){
        // Draw right wall
        // TODO: to make a multi player modezs
        wall.x = 1024 - thickness;
        wall.y = 0;
        wall.w = thickness;
        wall.h = 1024;
        SDL_RenderFillRect(mRenderer, &wall);
    }
    
	
	// Draw paddle
	SDL_Rect left_paddle{
		static_cast<int>(left_mPaddlePos.x),
		static_cast<int>(left_mPaddlePos.y - paddleH/2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &left_paddle);
	
    
    if(mMultiplePlayerMode==true){
        // Draw right paddle
        // TODO:  to add a multiplayer mode
        SDL_Rect right_paddle{
            static_cast<int>(right_mPaddlePos.x),
            static_cast<int>(right_mPaddlePos.y - paddleH/2),
            thickness,
            static_cast<int>(paddleH)
        };
        SDL_RenderFillRect(mRenderer, &right_paddle);
    }
    
    
    
	// Draw ball
	SDL_Rect ball{	
		static_cast<int>(mBallPos.x - thickness/2),
		static_cast<int>(mBallPos.y - thickness/2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball);
	
	// Swap front buffer and back buffer
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}
