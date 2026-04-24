#include "world.h"
#include <iostream>
#include <glad/glad.h>  
#include <GLFW/glfw3.h>
#include <cmath>
#include <algorithm>
World::World(vector<Body> bodys, float tf,double fd) :bodies(bodys), timefactor(tf),FrameDuration(fd) {
	sortBodies();
};
void World::updateBodies(float aspectRatio) {
	//cout << "speed x:" << bodies[0].speed.x << endl;
	


		for (int i = 0; i < bodies.size(); i++) {
			Body& nowBody = bodies[i];
			nowBody.acc = Point(0, 0);


			for (int j = 0; j < bodies.size();j++ ) {
				Body& otherBody = bodies[j];

				if (i != j && otherBody.shown !=false) {


					double dx = nowBody.postion.x - otherBody.postion.x;
					double dy = (nowBody.postion.y - otherBody.postion.y)/ aspectRatio;

					double r = sqrt(dx * dx + dy * dy);

					if (r <= (nowBody.redius + otherBody.redius ) ) {
						
						Body& bigger = (nowBody.mass >= otherBody.mass) ? nowBody : otherBody;
						Body& smaller = (nowBody.mass < otherBody.mass) ? nowBody : otherBody;

						double totalMass = bigger.mass + smaller.mass;
						bigger.speed.x = (bigger.speed.x * bigger.mass + smaller.speed.x * smaller.mass) / totalMass;
						bigger.speed.y = (bigger.speed.y * bigger.mass + smaller.speed.y * smaller.mass) / totalMass;

						bigger.mass = totalMass;
						bigger.redius = (0.002 + bigger.mass / 30000000);

						smaller.shown = false;

						if (!nowBody.shown) break;
					}
					bodies[i].applyGF(bodies[j], FrameDuration*timefactor);

				}
		      
			}
			
			Body& nowbody = bodies[i];
			if (nowbody.path.size() >= 199) {
				glBindVertexArray(nowbody.VAO);
				glBindBuffer(GL_ARRAY_BUFFER, nowbody.VBO);
				glBufferSubData(GL_ARRAY_BUFFER, 0, 200 * sizeof(float), nowbody.path.data());
				glDrawArrays(GL_LINE_STRIP, 0, 100);

				nowbody.path.erase(nowbody.path.begin());
				nowbody.path.erase(nowbody.path.begin());

			}
			nowbody.path.push_back(nowbody.postion.x);
			nowbody.path.push_back(nowbody.postion.y);








		}
		for (auto& body : bodies) {
			body.updatePhysics(FrameDuration*timefactor);
		}
		size_t oldSize = bodies.size();

		for (int i = 0; i < oldSize; i++) {
			if (!bodies[i].shown) {
				if (BodyFollow == i) {
					BodyFollow = -1;
				}
				glDeleteBuffers(1, &bodies[i].VBO);
				glDeleteVertexArrays(1, &bodies[i].VAO);






			}
		}

		bodies.erase(std::remove_if(bodies.begin(), bodies.end(),
			[](const Body& b) { return !b.shown; }),
			bodies.end());
	

		if (bodies.size() < oldSize) {
			sortBodies();

			updateGpuVram();
		}
	

}
void World::updateGpuVram() {
	vector<float> Masses = getBodiesMasses();
	glBindBuffer(GL_ARRAY_BUFFER, VBOS[1]);

	glBufferData(GL_ARRAY_BUFFER, Masses.size() * sizeof(float), Masses.data(), GL_DYNAMIC_DRAW);

	vector<float> positions = getBodiesPositions();
	glBindBuffer(GL_ARRAY_BUFFER, VBOS[2]);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_DYNAMIC_DRAW);

	vector<float> colors = getBodiesColors();
	glBindBuffer(GL_ARRAY_BUFFER, VBOS[3]);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_DYNAMIC_DRAW);

	vector<float> radiuses = getBodiesRadiuses();
	glBindBuffer(GL_ARRAY_BUFFER, VBOS[4]);
	glBufferData(GL_ARRAY_BUFFER, radiuses.size() * sizeof(float), radiuses.data(), GL_DYNAMIC_DRAW);
}



vector<float> World::getBodiesPositions() {
	vector<float> postions;
	for (int i = 0; i < bodies.size(); i++) {
		postions.push_back(bodies[i].postion.x);
		postions.push_back(bodies[i].postion.y);

	}
	return postions;
}

vector<float> World::getBodiesColors() {
	vector<float> colors;
	for (int i = 0; i < bodies.size(); i++) {
		colors.push_back(bodies[i].color[0]);
		colors.push_back(bodies[i].color[1]);
		colors.push_back(bodies[i].color[2]);
		colors.push_back(bodies[i].color[3]);

	}
	return colors;

}

vector<float> World::getBodiesMasses() {
	vector<float> Masses;
	for (int i = 0; i < bodies.size(); i++) {
		Masses.push_back(bodies[i].mass);
	}
	return Masses;

}
vector<float> World::getBodiesRadiuses() {
	vector<float> rediuses;
	for (int i = 0; i < bodies.size(); i++) {
		rediuses.push_back(bodies[i].redius);
	}
	return rediuses;

}
void World::sortBodies() {

	sort(bodies.begin(), bodies.end(), [](const Body body1, const Body body2) {
		return body1.mass < body2.mass;
		});
}