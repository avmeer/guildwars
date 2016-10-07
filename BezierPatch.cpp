
BezierPatch{
	private:
	vector<Point> points;
	
	public:
	BezierPatch();
	BezierPatch(vector<Point> points);
	createBezierPatch(vector<Point> points, u, v);
};


BezierPatch::BezierPatch(){}

BezierPatch::BezierPatch(vector<Point> *pointVector){
	points = pointVector;
}