#include"diamond.hpp"

Terrain::Terrain(){
	this->sea1=std::make_pair(0.00, 0.05);
	this->sea2=std::make_pair(0.05, 0.20);
	
	this->beach=std::make_pair(0.20, 0.25);

	this->grass1=std::make_pair(0.25, 0.35);
	this->grass2=std::make_pair(0.35, 0.40);
	
	this->froat1=std::make_pair(0.40, 0.50);
	this->froat2=std::make_pair(0.50, 0.55);
	
	this->mountain1=std::make_pair(0.55, 0.65);
	this->mountain2=std::make_pair(0.65, 1.00);
}

Terrain::~Terrain(){
}

double Terrain::random(const double min, const double max) const{
	std::random_device rnd;
	std::mt19937_64 mt(rnd());
	std::uniform_real_distribution<> rand(min, max);
	return rand(mt);
}

double Terrain::approximation2(const double val1, const double val2) const{
	double value=this->average2(val1, val2);
	double max=value+value*this->noise;
	double min=value-value*this->noise;
	while(true){
		double rand=this->random(min, max);
		if(rand>1)
			continue;
		return rand;
	}
}

double Terrain::approximation4(const double val1, const double val2, const double val3, const double val4) const{
	double value=this->average4(val1, val2, val3, val4);
	double max=value+value*this->noise;
	double min=value-value*this->noise;
	while(true){
		double rand=this->random(min, max);
		if(rand>1)
			continue;
		return rand;
	}
}

double Terrain::average2(const double val1, const double val2) const{
	return (val1+val2)/2;
}
double Terrain::average4(const double val1, const double val2, const double val3, const double val4) const{
	return (val1+val2+val3+val4)/4;
}

void Terrain::inits(){
	this->field.clear();
	this->field.resize(fieldSizeHeight);
	std::for_each(this->field.begin(), this->field.end(), [this](auto& vec){
			vec.resize(fieldSizeWight);
			std::fill(vec.begin(), vec.end(), 0.0);
		});
	std::for_each(this->field.begin()+1, this->field.end()-1, [this](auto& vec){
			std::for_each(vec.begin()+1, vec.end()-1, [this](auto& val){
					val=this->random(0.2, 0.8);
				});
		});
	const unsigned x=fieldSizeWight/2+1;
	const unsigned y=fieldSizeHeight/2+1;
	this->field.at(y).at(x)=1.0;
}

void Terrain::clear(){
	this->field.clear();
}

void Terrain::print() const{
	if(this->field.empty())
		return;
	static const double val=cellSize/2;
	static const unsigned pointSize=cellSize-1;
	glPointSize(pointSize);
	glBegin(GL_POINTS);
	for(int i=0;i<fieldSizeHeight;i++){
		for(int j=0;j<fieldSizeWight;j++){
			double state=this->field.at(i).at(j);
			
			if(state>=this->sea1.first&&state<this->sea1.second){
				glColor3f(0.0f, 0.0f, 1.0f);
				glVertex2d(val+cellSize*j, val+cellSize*i);
				continue;
			}
			if(state>=this->sea2.first&&state<this->sea2.second){
				glColor3f(0.0f, 0.3f, 1.0f);
				glVertex2d(val+cellSize*j, val+cellSize*i);
				continue;
			}
			
			if(state>=this->beach.first&&state<this->beach.second){
				glColor3f(1.0f, 1.0f, 0.5f);				
				glVertex2d(val+cellSize*j, val+cellSize*i);
				continue;				
			}
			
			if(state>=this->grass1.first&&state<this->grass1.second){
				glColor3f(0.0f, 1.0f, 0.0f);				
				glVertex2d(val+cellSize*j, val+cellSize*i);
				continue;				
			}
			if(state>=this->grass2.first&&state<this->grass2.second){
				glColor3f(0.0f, 0.8f, 0.0f);				
				glVertex2d(val+cellSize*j, val+cellSize*i);
				continue;				
			}
			
			if(state>=this->froat1.first&&state<this->froat1.second){
				glColor3f(0.0f, 0.5f, 0.0f);				
				glVertex2d(val+cellSize*j, val+cellSize*i);
				continue;				
			}
			if(state>=this->froat2.first&&state<this->froat2.second){
				glColor3f(0.0f, 0.3f, 0.0f);				
				glVertex2d(val+cellSize*j, val+cellSize*i);
				continue;				
			}
			
			if(state>=this->mountain1.first&&state<=this->mountain1.second){
				glColor3f(0.7f, 0.7f, 0.7f);
				glVertex2d(val+cellSize*j, val+cellSize*i);
				continue;				
			}
			if(state>=this->mountain2.first&&state<=this->mountain2.second){
				glColor3f(0.9f, 0.9f, 0.9f);
				glVertex2d(val+cellSize*j, val+cellSize*i);
				continue;				
			}
		}
	}
	glEnd();
}

double Terrain::get(const point &coord) const{
	if(this->field.empty())
		return 0;
	double state=this->field.at(coord.second).at(coord.first);
	if(state>=this->sea1.first&&state<this->sea1.second)
		return 3;
		//		return (this->sea1.first+this->sea1.second)/2*this->weight;
	if(state>=this->sea2.first&&state<this->sea2.second)
		return 2;
		//		return (this->sea2.first+this->sea2.second)/2*this->weight;
	
	if(state>=this->beach.first&&state<this->beach.second)
		return 1;
		//		return (this->beach.first+this->beach.second)/2*this->weight;
	
	if(state>=this->grass1.first&&state<this->grass1.second)
		return 0;
		//		return (this->grass1.first+this->grass1.second)/2*this->weight;
	if(state>=this->grass2.first&&state<this->grass2.second)
		return 1;
		//		return (this->grass2.first+this->grass2.second)/2*this->weight;
		
	if(state>=this->froat1.first&&state<this->froat1.second)
		return 2;
		//		return (this->froat1.first+this->froat1.second)/2*this->weight;
	if(state>=this->froat2.first&&state<this->froat2.second)
		return 3;
		//		return (this->froat2.first+this->froat2.second)/2*this->weight;
	
	if(state>=this->mountain1.first&&state<=this->mountain1.second)
		return 4;
		//		return (this->mountain1.first+this->mountain1.second)/2*this->weight;
	if(state>=this->mountain2.first&&state<=this->mountain2.second)
		return 5;
		//		return (this->mountain2.first+this->mountain2.second)/2*this->weight;
	
	return false;
}

void Terrain::diamondSquare(){
	const unsigned x=fieldSizeWight/2+1;
	const unsigned y=fieldSizeHeight/2+1;
	const point top=std::make_pair(x, y);
	if(this->field.empty())
		this->inits();
	for(int i=2;i<fieldSizeHeight-2;i++){
		for(int j=2;j<fieldSizeWight-2;j++){
			point currentCoord=std::make_pair(j, i);
			double currentValue=this->field.at(currentCoord.second).at(currentCoord.first);
			if(currentCoord==top)
				continue;
			for(int k=0;k<this->orthogonal1.size();k++){
				point changeCoord=currentCoord+this->orthogonal1.at(k);
				if(changeCoord==top)
					continue;
				point nextCoord=currentCoord+this->orthogonal2.at(k);
				double nextValue=this->field.at(nextCoord.second).at(nextCoord.first);
				double value=this->approximation2(currentValue, nextValue);
				this->field.at(changeCoord.second).at(changeCoord.first)=value;
			}
			for(int k=0;k<this->diagonal.size();k++){
				point changeCoord=currentCoord+this->diagonal.at(k);
				if(changeCoord==top)
					continue;				
				point nextCoord1=changeCoord+this->orthogonal1.at(0);
				point nextCoord2=changeCoord+this->orthogonal1.at(1);
				point nextCoord3=changeCoord+this->orthogonal1.at(2);
				point nextCoord4=changeCoord+this->orthogonal1.at(3);
				double nextValue1=this->field.at(nextCoord1.second).at(nextCoord1.first);
				double nextValue2=this->field.at(nextCoord2.second).at(nextCoord2.first);
				double nextValue3=this->field.at(nextCoord3.second).at(nextCoord3.first);
				double nextValue4=this->field.at(nextCoord4.second).at(nextCoord4.first);
				double value=this->approximation4(nextValue1, nextValue2, nextValue3, nextValue4);
				this->field.at(changeCoord.second).at(changeCoord.first)=value;
			};
		}
	}
}
