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
	if (Selfx + 2 >= 0 && Selfy - 1 >= 0) {
		potentialans.push_back((Selfy - 1) * grid->GetSizeX() + (Selfx + 2));
	}
	if (Selfx - 2 >= 0 && Selfy + 1 >= 0) {
		potentialans.push_back((Selfy + 1) * grid->GetSizeX() + (Selfx - 2));
	}
	if (Selfx + 2 >= 0 && Selfy + 1 >= 0) {
		
		potentialans.push_back((Selfy + 1) * grid->GetSizeX() + (Selfx + 2));
	}
	if (Selfx - 1 >= 0 && Selfy - 2 >= 0) {
		potentialans.push_back((Selfy - 2) * grid->GetSizeX() + (Selfx - 1));
	}
	if (Selfx + 1 >= 0 && Selfy - 2 >= 0) {
		potentialans.push_back((Selfy - 2) * grid->GetSizeX() + (Selfx + 1));
	}
	if (Selfx - 1 >= 0 && Selfy + 2 >= 0) {
		potentialans.push_back((Selfy + 2) * grid->GetSizeX() + (Selfx - 1));
	}
	if (Selfx + 1 >= 0 && Selfy + 2 >= 0) {
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
