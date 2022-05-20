#include "Interfaces.h"

 

std::vector<int> Rook::GetPotentialMovePositions(Grid* grid)
{
	std::vector<int>ans= std::vector<int>();
	std::vector<int>distances = grid->GetDistancesFromID(Selfy * grid->GetSizeX() + Selfx);
	for (int i = 0; i < 4; i++) {
		for (int j = 1; j < distances[i]+1; j++) {
			int curid = Selfy * grid->GetSizeX() + Selfx + grid->GetCoefficients()[i] * j;
			if (grid->GetCellFromInt(curid)->GetState() == CHCOLOR::Nothing) {
				ans.push_back(curid);
			}
			else if (grid->GetCellFromInt(curid)->GetState() == _color) {
				break;
			}
			else {
				ans.push_back(curid);
				break;
			}
			
		}
	}
	return ans;
	
}

std::vector<int> Knight::GetPotentialMovePositions(Grid* grid)
{
	std::vector<int> ans;
	std::vector<int> potentialans;
	if (Selfx - 2 >= 0 && Selfy - 1 >= 0) {
		potentialans.push_back((Selfy - 1) * grid->GetSizeX() + (Selfx-2));
	}
	if (Selfx + 2 <grid->GetSizeX() && Selfy - 1 >= 0) {
		potentialans.push_back((Selfy - 1) * grid->GetSizeX() + (Selfx + 2));
	}
	if (Selfx - 2 >= 0 && Selfy + 1 <grid->GetSizeY()) {
		potentialans.push_back((Selfy + 1) * grid->GetSizeX() + (Selfx - 2));
	}
	if (Selfx + 2 < grid->GetSizeX() && Selfy + 1 < grid->GetSizeY()) {
		
		potentialans.push_back((Selfy + 1) * grid->GetSizeX() + (Selfx + 2));
	}
	if (Selfx - 1 >= 0 && Selfy - 2 >= 0) {
		potentialans.push_back((Selfy - 2) * grid->GetSizeX() + (Selfx - 1));
	}
	if (Selfx + 1 < grid->GetSizeX() && Selfy - 2 >= 0) {
		potentialans.push_back((Selfy - 2) * grid->GetSizeX() + (Selfx + 1));
	}
	if (Selfx - 1 >= 0 && Selfy + 2 < grid->GetSizeY()) {
		potentialans.push_back((Selfy + 2) * grid->GetSizeX() + (Selfx - 1));
	}
	if (Selfx + 1 < grid->GetSizeX() && Selfy + 2 < grid->GetSizeY()) {
		potentialans.push_back((Selfy + 2) * grid->GetSizeX() + (Selfx + 1));
	}
	for (int i = 0; i < potentialans.size(); i++) {
		if (grid->GetCellFromInt(potentialans[i])->GetState() == CHCOLOR::Nothing) {
			ans.push_back(potentialans[i]);
		}
		else if (grid->GetCellFromInt(potentialans[i])->TakeFigureInfo()->GetColor() != GetColor()) {
			ans.push_back(potentialans[i]);
		}
	}
	return ans;
	 
}

std::vector<int> Bishop::GetPotentialMovePositions(Grid* grid)
{
	std::vector<int>ans = std::vector<int>();
	std::vector<int>distances = grid->GetDistancesFromID(Selfy * grid->GetSizeX() + Selfx);
	for (int i = 4; i < 8; i++) {
		for (int j = 1; j < distances[i] + 1; j++) {
			int zzz= grid->GetCoefficients()[i] * j;
			int curid = Selfy * grid->GetSizeX() + Selfx + zzz;
			if (grid->GetCellFromInt(curid)->GetState() == CHCOLOR::Nothing) {
				ans.push_back(curid);
			}
			else if (grid->GetCellFromInt(curid)->GetState() == _color) {
				break;
			}
			else {
				ans.push_back(curid);
				break;
			}

		}
	}
	return ans;
}

std::vector<int> Queen::GetPotentialMovePositions(Grid* grid)
{
	std::vector<int>ans = std::vector<int>();
	std::vector<int>distances = grid->GetDistancesFromID(Selfy * grid->GetSizeX() + Selfx);
	for (int i = 0; i < 8; i++) {
		for (int j = 1; j < distances[i] + 1; j++) {
			int zzz = grid->GetCoefficients()[i] * j;
			int curid = Selfy * grid->GetSizeX() + Selfx + zzz;
			if (grid->GetCellFromInt(curid)->GetState() == CHCOLOR::Nothing) {
				ans.push_back(curid);
			}
			else if (grid->GetCellFromInt(curid)->GetState() == _color) {
				break;
			}
			else {
				ans.push_back(curid);
				break;
			}

		}
	}
	return ans;
}

bool IsInBounds(int y, int x, Grid* g) {
	return (y >= 0 && y < g->GetSizeY()) && (x >= 0 && y < g->GetSizeX());
}

bool PawnDiagonalCanMove(int starty,int startx, int dify,int difx,CHCOLOR color, Grid* g) {
	//неизвестность
	//проверка на существование клетки
	if (!IsInBounds(starty + dify, startx + difx, g)) {
		return false;
	}
	//та клетка существует
	//проверить, стоит ли на той клетке союзник
	if (g->GetCellFromCoord(starty + dify, startx + difx)->GetState() == color) {
		return false;
	}
	//клетка существует и там нет соющников
	//проверить, стоит ли там враг
	if (g->GetCellFromCoord(starty + dify, startx + difx)->GetState() != CHCOLOR::Nothing) {
		return true;
	}
	//клетка существует и там пустота
	//проверить, стоит ли на стороне от нас враг
	Cell* checkedCell = g->GetCellFromCoord(starty, startx + difx);
	if (checkedCell->GetState() == CHCOLOR::Nothing || checkedCell->GetState() == color) {
		return false;
	}
	//со стороны от настоящей позиции враг
	//проверить, пешка ли этот враг
	IFigure* checkingFigure = checkedCell->TakeFigureInfo();
	if (checkingFigure->GetType() != Type::Pawn) {
		return false;
	}
	//со стороны от нас вражеская пешка
	//проверить, прыгала ли она в прошлом своем ходу
	if (static_cast<Pawn*>(checkingFigure)->TurnOfSprint == g->GetCurTurn() - 1) {
		return true;
	}
	//хз что до сюда может дойти
	return false;


}
int ConvertCoordToInt(int y, int x, Grid* g) {
	return y * g->GetSizeX() + x;
}
std::vector<int> Pawn::GetPotentialMovePositions(Grid* grid)
{
	std::vector<int> ans;

	int newY = (_color==CHCOLOR::Black) ?Selfy+1 :Selfy-1 ;
	
	if (IsInBounds(newY, Selfx, grid)) {
		if (grid->GetCellFromCoord(newY, Selfx)->GetState() ==CHCOLOR::Nothing) {
			ans.push_back(ConvertCoordToInt(newY, Selfx, grid));
		
			if (CurrentTurnPlayed == 1) {
				int nn = (_color == CHCOLOR::Black) ? Selfy + 2 : Selfy - 2;
				if (grid->GetCellFromCoord(nn, Selfx)->GetState() == CHCOLOR::Nothing) {
					ans.push_back(ConvertCoordToInt(nn, Selfx, grid));
				}
		
		
		}
		

	}
	if (PawnDiagonalCanMove(Selfy, Selfx, newY - Selfy, 1, _color, grid)) {
		ans.push_back(ConvertCoordToInt(newY, Selfx + 1, grid));
	}
	if (PawnDiagonalCanMove(Selfy, Selfx, newY - Selfy, -1, _color, grid)) {
		ans.push_back(ConvertCoordToInt(newY, Selfx - 1, grid));
	}
	//на клетке пустота
	
	}
	
	return ans;
}
/*
		проверить клетку перед собой, понятие перед мы определили+
		ввести функцию проверки диагональных клеток, туда спихнуть грид и координату
		ввести функцию для проверки, входят ли координаты в поле+
		вызвать эту функцию для клетки справа
		вызвать эту функцию для клетки слева
		проверить, первый ли это ход пешкой

		алгос:
		клетка впереди: проверка на вхождение, проверка на то, что на ней фигура не нашего цвета
		функция диагональных клеток:
			она подходит если:
				1) она в границах поля
				И (
				2) на ней стоит не наш цвет и не nothing
				3) на соседней с нами клетке стоит пешка, у которой ход джампа был текущий ход - 1

				параметры функции: текущая координата, смещение по x, смещение по y, цвет, грид


	*/
/*
	if (_color == CHCOLOR::Black) {


		if (Selfy + 1 < grid->GetSizeY()) {
			ans.push_back((Selfy+1) * grid->GetSizeX() + Selfx);
		}
		if (Selfy + 1 < grid->GetSizeY() && Selfx + 1 < grid->GetSizeX()) {
			if (grid->GetCellFromCoord(Selfy + 1, Selfx + 1)->GetState() == CHCOLOR::White) {
				ans.push_back((Selfy + 1) * grid->GetSizeX() + Selfx + 1);
			}
			else {
				if ( grid->GetCellFromCoord(Selfy, Selfx + 1)->GetState() == CHCOLOR::White) {
					if (grid->GetCellFromCoord(Selfy, Selfx + 1)->TakeFigureInfo()->GetType() == Type::Pawn) {
						if (static_cast<Pawn*>(grid->GetCellFromCoord(Selfy, Selfx + 1)->TakeFigureInfo())->TurnOfSprint == grid->GetCurTurn() - 1) {
							ans.push_back((Selfy+1) * grid->GetSizeX() + Selfx + 1);
						}
					}
				}
			}

		}
		if (Selfy + 1 < grid->GetSizeY() && Selfx - 1 >0) {
			if (grid->GetCellFromCoord(Selfy + 1, Selfx - 1)->GetState() == CHCOLOR::White) {
				ans.push_back((Selfy + 1) * grid->GetSizeX() + Selfx - 1);
			}
			else {
				if (Selfy + 1 == grid->GetSizeY() - 3 && grid->GetCellFromCoord(Selfy, Selfx - 1)->GetState() == CHCOLOR::White) {
					if (grid->GetCellFromCoord(Selfy, Selfx - 1)->TakeFigureInfo()->GetType() == Type::Pawn) {
						if (static_cast<Pawn*>(grid->GetCellFromCoord(Selfy, Selfx - 1)->TakeFigureInfo())->TurnOfSprint == grid->GetCurTurn() - 1) {
							ans.push_back((Selfy + 1) * grid->GetSizeX() + Selfx - 1);
						}
					}
				}
			}

		}
		if (CurrentTurnPlayed == 1) {
			ans.push_back((Selfy + 2) * grid->GetSizeX() + Selfx);
		}
	}
	else {

		if (Selfy - 1 >0) {
			ans.push_back((Selfy - 1) * grid->GetSizeX() + Selfx);
		}
		if (Selfy - 1 > 0 && Selfx + 1 < grid->GetSizeX()) {
			if (grid->GetCellFromCoord(Selfy - 1, Selfx + 1)->GetState() == CHCOLOR::Black) {
				ans.push_back((Selfy - 1) * grid->GetSizeX() + Selfx + 1);
			}
			else {
				if ( grid->GetCellFromCoord(Selfy, Selfx + 1)->GetState() == CHCOLOR::Black) {
					if (grid->GetCellFromCoord(Selfy, Selfx + 1)->TakeFigureInfo()->GetType() == Type::Pawn) {
						if (static_cast<Pawn*>(grid->GetCellFromCoord(Selfy, Selfx + 1)->TakeFigureInfo())->TurnOfSprint == grid->GetCurTurn() - 1) {
							ans.push_back((Selfy - 1) * grid->GetSizeX() + Selfx + 1);
						}
					}
				}
			}

		}
		if (Selfy - 1 > 0 && Selfx - 1 > 0) {
			if (grid->GetCellFromCoord(Selfy - 1, Selfx - 1)->GetState() == CHCOLOR::Black) {
				ans.push_back((Selfy + 1) * grid->GetSizeX() + Selfx - 1);
			}
			else {
				if(grid->GetCellFromCoord(Selfy, Selfx - 1)->GetState() == CHCOLOR::Black) {
					if (grid->GetCellFromCoord(Selfy, Selfx - 1)->TakeFigureInfo()->GetType() == Type::Pawn) {
						if (static_cast<Pawn*>(grid->GetCellFromCoord(Selfy, Selfx - 1)->TakeFigureInfo())->TurnOfSprint == grid->GetCurTurn() - 1) {
							ans.push_back((Selfy - 1) * grid->GetSizeX() + Selfx - 1);
						}
					}
				}
			}

		}
		if (CurrentTurnPlayed == 1) {
			ans.push_back((Selfy - 2) * grid->GetSizeX() + Selfx);
		}
	}
	return ans;
	*/
IFigure* FigureFactory::ConstructFigure(CHCOLOR col, Type tp)
{
	{
		switch (tp)
		{
		case Type::Pawn:
			return new Pawn(col);
			break;
		case Type::Rook:
			return new Rook(col);
			break;
		case Type::Knight:
			return new Knight(col);
			break;
		case Type::Bishop:
			return new Bishop(col);
			break;
		case Type::Queen:
			return new Queen(col);
			break;
		case Type::King:
			return new King(col);
			break;
		default:
			break;
		}
	}
}

std::vector<int> King::GetPotentialMovePositions(Grid* grid)
{
	std::vector<int> ans;
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			if (i == 0 && j == 0) {
				continue;
			}
			if (IsInBounds(Selfy + i, Selfx + j, grid)&& grid->GetCellFromCoord(Selfy+i,Selfx+j)->GetState()!=_color) {
				ans.push_back(ConvertCoordToInt(Selfy + i, Selfx + j, grid));
			}
		}
	}
	return ans;
}

IFigure* IFigure::Clone()
{
	return FigureFactory::ConstructFigure(_color, _type);
}
