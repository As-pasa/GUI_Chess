#pragma once
#include<iostream>
#include <SFML/Graphics.hpp>
#include<Windows.h>
#include<vector>

enum class CHCOLOR{Black=0,White=6,Nothing};
enum class Type{Pawn=0,Rook=1,Knight=2,Bishop=3,Queen=4,King=5};
class Grid;

class IFigure {
protected:
	CHCOLOR _color;
	Type _type;
	int Selfy = 0;
	int Selfx = 0;
public:
	void ChangePos(int y, int x) { Selfy = y; Selfx= x; }
	  std::pair<int, int> GetPos() { return { Selfy,Selfx }; }
	  CHCOLOR GetColor() { return _color; };
	  Type GetType() { return _type; }
	  virtual std::vector<int> GetPotentialMovePositions(Grid* grid)=0;
	  bool CanGoThere(int id,Grid* grid) {
		  std::vector<int> a = GetPotentialMovePositions(grid);
		  for (int i = 0; i < a.size(); i++) {
			  if (id == a[i]){ return true; };
		  }
		  return false;
	  }
	
};

class Rook :public IFigure {
public:
	Rook(CHCOLOR col  ) { _color = col; _type = Type::Rook; }
	std::vector<int> GetPotentialMovePositions(Grid* grid);
};
class Knight :public IFigure {
public:
	Knight(CHCOLOR col) { _color = col; _type = Type::Knight; }
	std::vector<int> GetPotentialMovePositions(Grid* grid);
};
class Bishop :public IFigure {
public:
	Bishop(CHCOLOR col) { _color = col; _type = Type::Bishop; }
	std::vector<int> GetPotentialMovePositions(Grid* grid);
};
class Queen :public IFigure {
public:
	Queen(CHCOLOR col) { _color = col; _type = Type::Queen; }
	std::vector<int> GetPotentialMovePositions(Grid* grid);
};

class DistanceHolder {
private:
	std::vector<std::vector<int>> _distances;
	std::vector<int> _coefficients;
	std::pair<int, int> _sizes;
public:
	DistanceHolder(int szy, int szx) {
		_sizes = { szy,szx };
		_coefficients = { -szx,1,szx,-1,-szx + 1,+szx + 1,+szx - 1,-szx - 1 };

		for (int i = 0; i < szy * szx; i++) {
			int LY = i / szx;
			int LX = i % szx;
			std::vector<int> localdistanes = { LY,szx - 1 - LX,szy - 1 - LY,LX,
				(LY < szx - 1 - LX) ? LY : szx - 1 - LX,
				(szx - 1 - LX < szy - 1 - LY) ? szx - 1 - LX : szy - 1 - LY,
				(szy - 1 - LY < LX) ? szy - 1 - LY : LX ,
				(LX < LY) ? LX : LY };
			_distances.push_back(localdistanes);
		}
	}
	std::vector<int>& GetDistancesFromID(int id) {
		return _distances[id];
	}
	std::vector<int>& GetDistancesFromCoord(int y, int x) {
		return _distances[y * _sizes.second + x];
	}
	const std::vector<int>& GetCoefficients() { return _coefficients; }
 };

class Cell {
private:
	
	IFigure* _figure=nullptr;
public:
	CHCOLOR _state;
	Cell() { _state = CHCOLOR::Nothing; }
	CHCOLOR GetState() { return _state; }
	void PutFigure(IFigure* f) {
		_figure = f;
		_state = f->GetColor();
	}
	IFigure* RemoveFigure() {
		_state = CHCOLOR::Nothing;
		return _figure;
	}
	IFigure *const TakeFigureInfo() {
		return _figure;
	}

};

class Grid {
private:
	int SizeX;
	int SizeY;
	std::vector<Cell*> _data;
	DistanceHolder _dists;
	int scaleFactor = 50;
public:
	Grid(int szy, int szx):_dists(szy,szx),SizeX(szx),SizeY(szy) {
		for (int i = 0; i < szy * szx; i++) {
			_data.push_back(new Cell());
		}
	};
	Cell* GetCellFromInt(int x) { return _data[x]; };
	Cell* GetCellFromCoord(int y,int x) {
		return _data[SizeX * y + x];
	}
	int GetSizeX() { return SizeX; }
	int GetSizeY() { return SizeY; }
	std::vector<int>& GetDistancesFromID(int id) { return _dists.GetDistancesFromID(id); }
	const std::vector<int>& GetCoefficients() { return _dists.GetCoefficients(); }
	~Grid() {
		for (int i = 0; i < _data.size(); i++)
		{
			if (_data[i]->GetState() != CHCOLOR::Nothing) {
				delete _data[i]->RemoveFigure();
			}
		}
	}
	void Put(IFigure* f,int id) {
		f->ChangePos(id / GetSizeX(), id % GetSizeX());
		_data[id]->PutFigure(f);
	}
	IFigure* RemoveFigure(int id) {
		return _data[id]->RemoveFigure();
	}
	IFigure* const GetFigureInfo(int id) {
		return _data[id]->TakeFigureInfo();
	}
	int GetScaleFactor() { return scaleFactor; }
};

class DisplayManager {
private:
	std::vector<sf::Texture> Textures;
	std::vector<sf::Sprite> FigureSprites;
	Grid* _grid;
	float scaleFactor = 50.f;
 
	
	sf::RectangleShape HighlightStaff;
	std::vector<sf::RectangleShape> _fill;
	std::vector<int> _Highlights;
public:
	DisplayManager(Grid* g):_grid(g) {
		sf::Texture a;
		a.loadFromFile("b_pawn_1x_ns.png");
		Textures.push_back(a);
		a.loadFromFile("b_rook_1x_ns.png");
		Textures.push_back(a);
		a.loadFromFile("b_knight_1x_ns.png");
		Textures.push_back(a);
		a.loadFromFile("b_bishop_1x_ns.png");
		Textures.push_back(a);
		a.loadFromFile("b_queen_1x_ns.png");
		Textures.push_back(a);
		a.loadFromFile("b_king_1x_ns.png");
		Textures.push_back(a);
		a.loadFromFile("w_pawn_1x_ns.png");
		Textures.push_back(a);
		a.loadFromFile("w_rook_1x_ns.png");
		Textures.push_back(a);
		a.loadFromFile("w_knight_1x_ns.png");
		Textures.push_back(a);
		a.loadFromFile("w_bishop_1x_ns.png");
		Textures.push_back(a);
		a.loadFromFile("w_queen_1x_ns.png");
		Textures.push_back(a);
		a.loadFromFile("w_king_1x_ns.png");
		Textures.push_back(a);
		for (int i = 0; i < Textures.size(); i++) {
			sf::Sprite x;
			x.setTexture(Textures[i]);
			x.setScale(scaleFactor / a.getSize().x, scaleFactor / a.getSize().y);
			FigureSprites.push_back(x);
		}
		HighlightStaff.setSize({ scaleFactor,scaleFactor});
		HighlightStaff.setFillColor(sf::Color::Green);

	 
		
		 

		for (int i = 0; i < _grid->GetSizeY(); i++)
		{
			for (int j = 0; j < _grid->GetSizeX(); j++)
			{
				sf::RectangleShape xx;
				xx.setPosition(j * scaleFactor, i * scaleFactor);
				xx.setSize({ scaleFactor, scaleFactor});
				xx.setFillColor(((i + j) % 2 == 0) ? sf::Color::White : sf::Color::Blue);
				_fill.push_back(xx);
			}
		}
	}
	void HighlightWays(IFigure* f,Grid* g) {
		std::vector<int> poses = f->GetPotentialMovePositions(_grid);
		for (int i = 0; i < poses.size();i++) {
			_Highlights.push_back(poses[i]);
		}
	};
	void ClearHightLights() {
		_Highlights.clear();
	};
	void DrawAll(sf::RenderWindow& window) {
		 
		
		for (int i = 0; i < _fill.size(); i++)
		{
			window.draw(_fill[i]);
		}
		for (int i = 0; i < _Highlights.size(); i++) {
			HighlightStaff.setPosition((_Highlights[i] % _grid->GetSizeX()) * scaleFactor, (_Highlights[i] / _grid->GetSizeX()) * scaleFactor);
			window.draw(HighlightStaff);
		}
		for (int i = 0; i < _grid->GetSizeX() * _grid->GetSizeY(); i++) {
			if (_grid->GetCellFromInt(i)->GetState() != CHCOLOR::Nothing) {
				IFigure* cur = _grid->GetCellFromInt(i)->TakeFigureInfo();
				int curTexture = static_cast<int>(cur->GetColor()) + static_cast<int> (cur->GetType());
				FigureSprites[curTexture].setPosition((i % _grid->GetSizeX()) * scaleFactor, (i / _grid->GetSizeX()) * scaleFactor);
				window.draw(FigureSprites[curTexture]);
			}
		}
		
	};
};

class InputManager {
	int CellID;
	bool _UserClicked = false;
	
	 


	int SelectedCellID = -1;
	int LastPushedID=-1;
	int LastReleasedID=-2;
public:
	void UpdateInut(double coordy, double coordx,Grid* g) {
		
 
		if (coordx < 0) { coordx = 0; }
		if (coordy < 0) { coordy = 0; }
		int y = coordy / g->GetScaleFactor();
		int x = coordx / g->GetScaleFactor();
		 
		CellID= y * g->GetSizeX() + x;
	}
	void SetIsPressed(bool pp) {
		_UserClicked = pp;
	}
	void UpdateSelected() {
		
		SelectedCellID = -1;
		if (_UserClicked == true) {
			LastPushedID = CellID;
		}
		if (_UserClicked == false) {
			LastReleasedID = CellID;
		} 
		//std::cout << LastPushedID << " " << LastReleasedID << "\n";
		if (LastPushedID == LastReleasedID && LastReleasedID != -1) {
			SelectedCellID = LastPushedID;
			LastPushedID = -1;
			LastReleasedID = -1;
		}
	}
 
	int GetCell() { 
		return SelectedCellID;
	}
	 
};


enum class TurnState{WaitingForFirstCoord,WaitingForSecondCoord,CheckingTurn,EndOfTurn};
enum class GameState{WhiteTurn,BlackTurn,EndOfGame};
class Player {
private:
	CHCOLOR Color;
	int SelectedFirstCell = -1;
	int SelectedSecondCell = -1;
	TurnState st = TurnState::WaitingForFirstCoord;
public:	
	Player(CHCOLOR c) :Color(c) {};
	void Update(InputManager& _input,Grid* _grid,DisplayManager& _display) {
		_input.UpdateSelected();

		switch (st)
		{
		case TurnState::WaitingForFirstCoord:
			if (_input.GetCell() != -1) {
				SelectedFirstCell = _input.GetCell();
				if (_grid->GetCellFromInt(SelectedFirstCell)->GetState() == Color) {
					std::cout << "FigureSelected\n";
					_display.HighlightWays(_grid->GetCellFromInt(SelectedFirstCell)->TakeFigureInfo(), _grid);
					st = TurnState::WaitingForSecondCoord;
				}

			}
			break;
		case TurnState::WaitingForSecondCoord:
			if (_input.GetCell() != -1 && _input.GetCell() != SelectedFirstCell) {

				SelectedSecondCell = _input.GetCell();
				if (_grid->GetCellFromInt(SelectedFirstCell)->TakeFigureInfo()->CanGoThere(SelectedSecondCell, _grid)) {
					std::cout << "TargetSelected\n";
					st = TurnState::CheckingTurn;
				}
				else {
					SelectedFirstCell = -1;
					SelectedSecondCell = -1;
					st = TurnState::WaitingForFirstCoord;
					_display.ClearHightLights();
				}
			}
			break;
		case TurnState::CheckingTurn:
			std::cout << "EndIsComing\n";
			_grid->Put(_grid->GetCellFromInt(SelectedFirstCell)->RemoveFigure(), SelectedSecondCell);


			_display.ClearHightLights();
			st = TurnState::EndOfTurn;
			break;
		case TurnState::EndOfTurn:
			SelectedFirstCell = -1;
			SelectedSecondCell = -1;
			st = TurnState::WaitingForFirstCoord;
			break;
		default:
			break;
		}
		
	}
	TurnState GetState() { return st; }
};




class LogicManager {
private:
	int GridSizeX=8;
	int GridSizeY=8;
	Grid* _grid;
	Player black;
	Player white;
	GameState state;
	DisplayManager _display;
public:
	LogicManager():_grid(new Grid(8,8)),_display(_grid),black(CHCOLOR::Black),white(CHCOLOR::White) {};
	InputManager _input;
	void Init() {
		
		GridSizeX = 8;
		GridSizeY = 8;
		Bishop* x = new Bishop(CHCOLOR::Black);
		Rook* y = new Rook(CHCOLOR::White);
		Rook* z = new Rook(CHCOLOR::Black);
		_grid->Put(x, 10);
		_grid->Put(y, 19);
		_grid->Put(z, 40);
	}
	
	void Test(sf::RenderWindow& w) {
		 
		switch (state)
		{
		case GameState::WhiteTurn:
			white.Update(_input, _grid, _display);
			if (white.GetState() == TurnState::EndOfTurn) {
				state = GameState::BlackTurn;
			}
			break;
		case GameState::BlackTurn:
			black.Update(_input, _grid, _display);
			if (black.GetState() == TurnState::EndOfTurn) {
				state = GameState::WhiteTurn;
			}
			break;
		case GameState::EndOfGame:
			break;
		default:
			break;
		}


		
		 
		_display.DrawAll(w);
		
	}
};
