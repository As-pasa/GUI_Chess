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
	int CurrentTurnPlayed = 0;
	virtual IFigure* Clone();
	virtual void ChangePos(int y, int x, Grid* g) { Selfy = y; Selfx = x; CurrentTurnPlayed++; }
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

class FigureFactory {
public:
	static IFigure* ConstructFigure(CHCOLOR col, Type tp);
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
	int CurrentTurn = 0;
public:
	Grid(int szy, int szx,int curturn=0):_dists(szy,szx),SizeX(szx),SizeY(szy),CurrentTurn(curturn) {
		for (int i = 0; i < szy * szx; i++) {
			_data.push_back(new Cell());
		}
	};
	Grid(Grid& other):_dists(other._dists) {
		scaleFactor = other.scaleFactor;
		SizeX = other.SizeX;
		SizeY = other.SizeY;
		CurrentTurn = other.CurrentTurn;
		SetData(other.GetData());

	}
	Cell* GetCellFromInt(int x) { return _data[x]; };
	Cell* GetCellFromCoord(int y,int x) {
		return _data[SizeX * y + x];
	}
	int GetSizeX() { return SizeX; }
	int GetSizeY() { return SizeY; }
	std::vector<int>& GetDistancesFromID(int id) { return _dists.GetDistancesFromID(id); }
	const std::vector<int>& GetCoefficients() { return _dists.GetCoefficients(); }
	~Grid() {
		Flush();
	}
	void Flush() {
		for (int i = 0; i < _data.size(); i++)
		{
			if (_data[i]->GetState() != CHCOLOR::Nothing) {
				delete _data[i]->RemoveFigure();
				delete _data[i];
			}
		}
		_data.clear();
	}
	std::vector<Cell*> GetData() { return _data; }
	void SetData(std::vector<Cell*> a) {
		for (int i = 0; i < a.size(); i++) {
			_data.push_back(new Cell(*a[i]));
			if (_data[i]->GetState() != CHCOLOR::Nothing) {
				 _data[i]->TakeFigureInfo()->ChangePos(i/SizeX,i%SizeX,this);
			}

		}
	}
	void Put(IFigure* f,int id) {
		f->ChangePos(id / GetSizeX(), id % GetSizeX(),this);
		_data[id]->PutFigure(f);
	}
	IFigure* RemoveFigure(int id) {
		_data[id]->TakeFigureInfo();
		return _data[id]->RemoveFigure();
	}
	IFigure* const GetFigureInfo(int id) {
		return _data[id]->TakeFigureInfo();
	}
	int GetScaleFactor() { return scaleFactor; }
	void EndTurn() { CurrentTurn++; }
	int GetCurTurn() { return CurrentTurn; }
};

class Pawn :public IFigure {
public:
	int TurnOfSprint = -1;

	Pawn(CHCOLOR col) { _color = col; _type = Type::Pawn; }
	IFigure* Clone() {
		IFigure* ans = FigureFactory::ConstructFigure(_color, _type);
		Pawn* aa = static_cast<Pawn*>(ans);
		aa->CurrentTurnPlayed = CurrentTurnPlayed;
		aa->TurnOfSprint = TurnOfSprint;
		return ans;
	}
	void ChangePos(int y, int x, Grid* g) {
		if (y - Selfy == 2 || y - Selfy == -2) {
			TurnOfSprint = g->GetCurTurn();
		}
		Selfy = y;
		Selfx = x;
		CurrentTurnPlayed++;
	}
	std::vector<int>GetPotentialMovePositions(Grid* grid);

};

class King :public IFigure {
public:
	King(CHCOLOR col) {
		_color = col; _type = Type::King;
	}
	int TurnOfRokirovka = -1;

	IFigure* Clone() {
		IFigure* ans = FigureFactory::ConstructFigure(_color, _type);
		King* aa = static_cast<King*>(ans);
		aa->TurnOfRokirovka = TurnOfRokirovka;
		aa->CurrentTurnPlayed = CurrentTurnPlayed;
		return ans;
	}
	std::vector<int> GetPotentialMovePositions(Grid* grid);
};

class DisplayManager {
private:
	std::vector<sf::Texture> Textures;
	std::vector<sf::Sprite> FigureSprites;
	 
	float scaleFactor = 50.f;
 
	
	sf::RectangleShape HighlightStaff;
	std::vector<sf::RectangleShape> _fill;
	std::vector<int> _Highlights;
public:
	DisplayManager(Grid* g)  {
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

	 
		
		 

		for (int i = 0; i < g->GetSizeY(); i++)
		{
			for (int j = 0; j < g->GetSizeX(); j++)
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
		std::vector<int> poses = f->GetPotentialMovePositions(g);
		for (int i = 0; i < poses.size();i++) {
			_Highlights.push_back(poses[i]);
		}
	};
	void ClearHightLights() {
		_Highlights.clear();
	};
	void DrawAll(sf::RenderWindow& window,Grid* _grid) {
		 
		
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

class TurnChecker {
private:
	Grid* _startingGrid;
	Grid* _forkGrid;
	CHCOLOR _contextCol;
	int _startID;
	int _endID;
	int _kingPlace=-1;
	
	void ReplaceFigures(Grid* g) {
		if (g->GetCellFromInt(_endID)->GetState() != CHCOLOR::Nothing) {
			IFigure* destroyed = g->GetCellFromInt(_endID)->RemoveFigure();
			delete destroyed;
		}
		IFigure* mainHero = g->GetCellFromInt(_startID)->RemoveFigure();
		g->Put(mainHero, _endID);
		g->EndTurn();
	}
	void FindTheKing() {
		for (int i = 0; i < _forkGrid->GetSizeX() * _forkGrid->GetSizeY(); i++) {
			if (_forkGrid->GetCellFromInt(i)->GetState() == _contextCol) {
				if (_forkGrid->GetCellFromInt(i)->TakeFigureInfo()->GetType() == Type::King) {
					_kingPlace = i;
					return;
				}
			}
		}
	
	}
	bool AnyAlert() {
		Cell* AssasinSpot;
		if (_kingPlace == -1) {
			return false;
		}
		for (int i = 0; i < _forkGrid->GetSizeX() * _forkGrid->GetSizeY(); i++) {
		 
			
			AssasinSpot = _forkGrid->GetCellFromInt(i);
			if (AssasinSpot->GetState() != CHCOLOR::Nothing && AssasinSpot->GetState() != _contextCol) {
				IFigure* PotentialAssasin = AssasinSpot->TakeFigureInfo();
				if (PotentialAssasin->CanGoThere(_kingPlace, _forkGrid)) {
					return true;
				}
			}
		}
		return false;

	}
public:
	TurnChecker(Grid* startGrid, CHCOLOR col, int startCellID, int EndCellID) :_startingGrid(startGrid), _contextCol(col), _startID(startCellID), _endID(EndCellID) {
		_forkGrid = new Grid(_startingGrid->GetSizeY(), _startingGrid->GetSizeX(), _startingGrid->GetCurTurn());
		for (int i = 0; i < _startingGrid->GetSizeX() * _startingGrid->GetSizeY(); i++) {
			if (_startingGrid->GetCellFromInt(i)->GetState() != CHCOLOR::Nothing) {
				_forkGrid->Put(_startingGrid->GetCellFromInt(i)->TakeFigureInfo()->Clone(), i);
			}
		}
	}
	
	bool Check() {


		ReplaceFigures(_forkGrid);
		FindTheKing();
		return !AnyAlert();
	}
	~TurnChecker() {
		delete _forkGrid;
	}
};

enum class TurnState{WaitingForFirstCoord,WaitingForSecondCoord,CheckingTurn,EndOfTurn,PawnTransformation};
enum class GameState{WhiteTurn,BlackTurn,EndOfGame,WhiteChoosingFigure,BlachCoosingFigure};
class Player {
private:
	CHCOLOR Color;
	int SelectedFirstCell = -1;
	int SelectedSecondCell = -1;
	Type SelecteddTransformType = Type::Pawn;
	TurnState st = TurnState::WaitingForFirstCoord;
	bool CheckKingSafety() {
		return false;
	}
	bool CheckIfPawnNeedTransformation(Grid* gr) {
		if (gr->GetCellFromInt(SelectedSecondCell)->GetState() != Color) { return false; }
		if (gr->GetCellFromInt(SelectedSecondCell)->TakeFigureInfo()->GetType() != Type::Pawn) { return false; }
		int row = (Color == CHCOLOR::White) ? 0 : gr->GetSizeY() - 1;
		if (SelectedSecondCell / gr->GetSizeX() == row) {
			return true;
		}
		return false;

	}
public:	
	Player(CHCOLOR c) :Color(c) {};
	void Update(InputManager& _input,Grid* _grid,DisplayManager& _display,Grid* SelectGrid) {
		_input.UpdateSelected();
	
		TurnChecker* TheGuard;
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
			TheGuard=new TurnChecker(_grid, Color, SelectedFirstCell, SelectedSecondCell);
			
			if (TheGuard->Check()) {
				if (_grid->GetCellFromInt(SelectedSecondCell)->GetState() != CHCOLOR::Nothing) {
					delete _grid->GetCellFromInt(SelectedSecondCell)->RemoveFigure();

				}
				else if (_grid->GetCellFromInt(SelectedFirstCell)->TakeFigureInfo()->GetType() == Type::Pawn) {
					int coef = (Color == CHCOLOR::White) ? 1 : -1;
					if ((SelectedFirstCell - SelectedSecondCell) % _grid->GetSizeX() != 0) {
						IFigure* z = _grid->GetCellFromInt(SelectedSecondCell + _grid->GetSizeX() * coef)->TakeFigureInfo();
						if (z->GetColor() != Color && z->GetType() == Type::Pawn) {
							delete _grid->GetCellFromInt(SelectedSecondCell + _grid->GetSizeX() * coef)->RemoveFigure();
						}
					}
				}
				else if (_grid->GetCellFromInt(SelectedFirstCell)->TakeFigureInfo()->GetType() == Type::King) {
					King* z =static_cast<King*>(_grid->GetCellFromInt(SelectedFirstCell)->TakeFigureInfo());
					std::cout << SelectedSecondCell << "\n";
					if (SelectedSecondCell- SelectedFirstCell == 2) {
						z->TurnOfRokirovka = _grid->GetCurTurn();
						IFigure* Rook = _grid->GetCellFromCoord(z->GetPos().first, _grid->GetSizeX() - 1)->RemoveFigure();
						_grid->Put(Rook, SelectedFirstCell + 1);
					}
					else if (SelectedSecondCell-SelectedFirstCell == -2) {
						z->TurnOfRokirovka = _grid->GetCurTurn();
						IFigure* Rook = _grid->GetCellFromCoord(z->GetPos().first, 0)->RemoveFigure();
						_grid->Put(Rook, SelectedFirstCell - 1);
					}
				}
				_grid->Put(_grid->GetCellFromInt(SelectedFirstCell)->RemoveFigure(), SelectedSecondCell);
				_display.ClearHightLights();


				 

				//реальный конец хода
				if (CheckIfPawnNeedTransformation(_grid)) {
					std::cout << "firstCheckReady\n";
					st = TurnState::PawnTransformation;
				}
				else {
					st = TurnState::EndOfTurn;
					_grid->EndTurn();

				}
			
				
			}
			//перевыбор хода
			else {
				_display.ClearHightLights();
				SelectedFirstCell = -1;
				SelectedSecondCell = -1;
				st = TurnState::WaitingForFirstCoord;
			}
			
			delete TheGuard;
			break;
		case TurnState::EndOfTurn:
			SelectedFirstCell = -1;
			SelectedSecondCell = -1;
			st = TurnState::WaitingForFirstCoord;
			break;
		
		case TurnState::PawnTransformation:
			if (_input.GetCell() != -1) {
				if (_input.GetCell() < 4) {
					SelecteddTransformType = SelectGrid->GetCellFromInt(_input.GetCell())->TakeFigureInfo()->GetType();
				}
			}
			



			if (SelecteddTransformType != Type::Pawn) {
				
				IFigure* newFigure = FigureFactory::ConstructFigure(Color, SelecteddTransformType);
			
				delete _grid->GetCellFromInt(SelectedSecondCell)->RemoveFigure();
				_grid->Put(newFigure, SelectedSecondCell);
				st = TurnState::EndOfTurn;
				_grid->EndTurn();
			}

			break;
		
		
		default:
			break;
		}
		
	}
	TurnState GetState() { return st; }
};

class LogicManager {
private:
 
	Grid* _grid;
	Grid* _FigureSelectionGrid;
	Player black;
	Player white;
	GameState state;
	DisplayManager _display;
public:
	LogicManager():_grid(new Grid(8,8)),_display(_grid),black(CHCOLOR::Black),white(CHCOLOR::White),_FigureSelectionGrid(new Grid(8,8)) {};
	InputManager _input;
	Grid* GetGrid() {
		if (state == GameState::WhiteChoosingFigure || state == GameState::BlachCoosingFigure) {
			return _FigureSelectionGrid;
		}
		else {
			return _grid;
		}

	}
	void Init() {
		_FigureSelectionGrid->Put(FigureFactory::ConstructFigure(CHCOLOR::White, Type::Rook),0);
		_FigureSelectionGrid->Put(FigureFactory::ConstructFigure(CHCOLOR::White, Type::Knight), 1);
		_FigureSelectionGrid->Put(FigureFactory::ConstructFigure(CHCOLOR::White, Type::Bishop), 2);
		_FigureSelectionGrid->Put(FigureFactory::ConstructFigure(CHCOLOR::White, Type::Queen), 3);
		 
		_grid->Put(new King(CHCOLOR::White), (_grid->GetSizeY() - 1) * (_grid->GetSizeX()) + 4);
		_grid->Put(new Rook(CHCOLOR::White), (_grid->GetSizeY() - 1) * (_grid->GetSizeX()));
		_grid->Put(new Rook(CHCOLOR::White), (_grid->GetSizeY() - 1) * (_grid->GetSizeX()) + 7);
		_grid->Put(new Bishop(CHCOLOR::Black), _grid->GetSizeX() - 3);
		_grid->Put(new King(CHCOLOR::Black), 4);
		_grid->Put(new Pawn(CHCOLOR::White),62 );
		//_grid->Put(new Knight(CHCOLOR::Black), 20);
		//_grid->Put(new Knight(CHCOLOR::Black), 1);
		//_grid->Put(new Bishop(CHCOLOR::White), 40);
		/*
		for (int i = _grid->GetSizeX(); i < _grid->GetSizeX() * 2; i++) {
			_grid->Put(new Pawn(CHCOLOR::Black),i);
		};
		_grid->Put(new Rook(CHCOLOR::Black), 0);
		_grid->Put(new Rook(CHCOLOR::Black), _grid->GetSizeX()-1);
		_grid->Put(new Knight(CHCOLOR::Black), 1);
		_grid->Put(new Knight(CHCOLOR::Black), _grid->GetSizeX()-2);
		_grid->Put(new Bishop(CHCOLOR::Black), _grid->GetSizeX() - 3);
		_grid->Put(new Bishop(CHCOLOR::Black),2);
		_grid->Put(new Queen(CHCOLOR::Black), 3);
		_grid->Put(new King(CHCOLOR::Black), 4);
		
		_grid->Put(new Rook(CHCOLOR::White), (_grid->GetSizeY() - 1)*(_grid->GetSizeX()));
		_grid->Put(new Knight(CHCOLOR::White), (_grid->GetSizeY() - 1) * (_grid->GetSizeX())+1);
		_grid->Put(new Bishop(CHCOLOR::White), (_grid->GetSizeY() - 1) * (_grid->GetSizeX())+2);
		_grid->Put(new Queen(CHCOLOR::White), (_grid->GetSizeY() - 1) * (_grid->GetSizeX())+3);
		_grid->Put(new King(CHCOLOR::White), (_grid->GetSizeY() - 1) * (_grid->GetSizeX())+4);
		_grid->Put(new Bishop(CHCOLOR::White), (_grid->GetSizeY() - 1) * (_grid->GetSizeX())+5);
		_grid->Put(new Knight(CHCOLOR::White), (_grid->GetSizeY() - 1) * (_grid->GetSizeX()) + 6);
		_grid->Put(new Rook(CHCOLOR::White), (_grid->GetSizeY() - 1) * (_grid->GetSizeX()) + 7);
		for (int i = (_grid->GetSizeY() - 2) * (_grid->GetSizeX()); i < (_grid->GetSizeY() - 1) * (_grid->GetSizeX()); i++) {
			_grid->Put(new Pawn(CHCOLOR::White), i);
		}
		*/
		/*_grid->Put(new Rook(CHCOLOR::White), _grid->GetSizeX() - 1);
		_grid->Put(new Knight(CHCOLOR::White), 1);
		_grid->Put(new Knight(CHCOLOR::White), _grid->GetSizeX() - 2);
		_grid->Put(new Bishop(CHCOLOR::White), _grid->GetSizeX() - 3);
		_grid->Put(new Bishop(CHCOLOR::White), 2);
		_grid->Put(new Queen(CHCOLOR::White), 3);
		_grid->Put(new King(CHCOLOR::White), 4);
		*/

	}
	
	void Test(sf::RenderWindow& w) {
		Grid* nowgr = _grid;
		switch (state)
		{
		case GameState::WhiteTurn:
			white.Update(_input, _grid, _display, _FigureSelectionGrid);
			if (white.GetState() == TurnState::PawnTransformation) {
				state = GameState::WhiteChoosingFigure;
			}
			else if (white.GetState() == TurnState::EndOfTurn) {
				state = GameState::BlackTurn;
			}
			break;
		case GameState::BlackTurn:
			black.Update(_input, _grid, _display, _FigureSelectionGrid);
			if (black.GetState() == TurnState::PawnTransformation) {
				state = GameState::BlachCoosingFigure;
			}
			if (black.GetState() == TurnState::EndOfTurn) {
				state = GameState::WhiteTurn;
			}
			break;

		case GameState::BlachCoosingFigure:
			nowgr = _FigureSelectionGrid;
			black.Update(_input, _grid, _display, _FigureSelectionGrid);
			if (black.GetState() == TurnState::EndOfTurn) {
				state = GameState::WhiteTurn;
			}
			break;


		case GameState::WhiteChoosingFigure:
			nowgr = _FigureSelectionGrid;
			white.Update(_input, _grid, _display, _FigureSelectionGrid);
			if (white.GetState() == TurnState::EndOfTurn) {
				state = GameState::BlackTurn;
			}
			break;

		case GameState::EndOfGame:
			break;
		default:
			break;
		}




		_display.DrawAll(w, nowgr);

	};
};
/*
	Замена фигуры:
		 ироку перейти в специальное состояние
			до первой селектет фигуры
			получить фигуру
			заспавнить фигуру
			завершить ход
			игроку выйти из специального состояния
		 
		 всей игре перейти в специальное состояние
			рендерить и передавать игроку другое поле
	
		п
		игре выйти из специального состояния, вернуться в стандартный цикл


		
*/