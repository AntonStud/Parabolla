#pragma once
class Ball
{
private:

	POINT currCoord;

	double velocity;

	int degree;

	double multVelosCos;

	double multVelosSin;

	const double HALF_G, PI;



public:
	Ball();
	~Ball();

	POINT &GetCurrCoord();

	void ComputeCoord(const int &t);

	void ComputeVelosCos();

	void ComputeVelosSin();

	void SetVeloc(const double &v);

	void SetDegree(const int &d);

	double GetmultVelosCos() const;



};

