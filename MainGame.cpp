#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"

int DISPLAY_WIDTH = 640;
int DISPLAY_HEIGHT = 360;
int DISPLAY_SCALE = 1;

struct GameState
{
	float elapsedTime = 0;
};
GameState gameState;

enum GameObjectType
{
	TYPE_BALL,
	TYPE_PADDLE,
	TYPE_BRICK,
};
void HandlePlayerControls();
void UpdateBall();
void UpdateBrick();

void SetupScene()
{
	const int brickWidth = 18;
	const int brickHeight = 12;
	const int numCols = 33;
	float margin = (DISPLAY_WIDTH - (numCols * brickWidth))*0.7; //i dont know why 0.7 was needed but it works
	

	int numRows = (DISPLAY_HEIGHT / 4)/brickHeight;


	for (int row = 0; row < numRows; ++row)
	{
		for (int col = 0; col < numCols; ++col)
		{
			float X = margin + col * brickWidth;
			float Y = margin + row * brickHeight;

			int brickId = Play::CreateGameObject(TYPE_BRICK, {X, Y }, 6, "brick");
		}
	}

}

// The entry point for a PlayBuffer program
void MainGameEntry( PLAY_IGNORE_COMMAND_LINE )
{
	Play::CreateManager( DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE );
	Play::CentreAllSpriteOrigins();
	
	int id_ball= Play::CreateGameObject(TYPE_BALL, { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT - 60 }, 4, "ball");
	Play::GetGameObject(id_ball).velocity = { 1,-5 };

	SetupScene();
	
}

// Called by PlayBuffer every frame (60 times a second!)
bool MainGameUpdate( float elapsedTime )
{
	HandlePlayerControls();
	Play::ClearDrawingBuffer( Play::cBlack );

	UpdateBall();
	UpdateBrick();

	Play::PresentDrawingBuffer();
	return Play::KeyDown( VK_ESCAPE );
}

// Gets called once when the player quits the game 
int MainGameExit( void )
{
	Play::DestroyManager();
	return PLAY_OK;
}

void HandlePlayerControls()
{
	GameObject& obj_paddle = Play::GetGameObjectByType(TYPE_PADDLE);
	//IMPLEMENT LATER
}

void UpdateBall()
{
	std::vector<int> ballIds = Play::CollectGameObjectIDsByType(TYPE_BALL);
	
	for (int id : ballIds)
	{
		//if(Play::IsColliding(obj_ball, obj_brick))

		GameObject& obj_ball = Play::GetGameObject(id);
		Play::UpdateGameObject(obj_ball);
		if (Play::IsLeavingDisplayArea(obj_ball, Play::VERTICAL) && obj_ball.pos.y < DISPLAY_HEIGHT / 2)
		{
			obj_ball.velocity.y = -obj_ball.velocity.y;
		}
		if (Play::IsLeavingDisplayArea(obj_ball, Play::HORIZONTAL))
		{
			obj_ball.velocity.x = -obj_ball.velocity.x;
		}
		Play::DrawObjectRotated(obj_ball);
	}
}

void UpdateBrick()
{
	std::vector<int> brickIds = Play::CollectGameObjectIDsByType(TYPE_BRICK);
	for (int id : brickIds)
	{
		GameObject& obj_brick = Play::GetGameObject(id);
		Play::DrawObject(obj_brick);
	}

}


