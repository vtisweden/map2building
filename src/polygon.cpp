#include "polygon.h"

void Polygon::push_back(osg::Vec2 point) {
	if (!m_points) {
		m_points = new osg::Vec2Array;
	}
	m_points->push_back(point); 
	m_dirtyPoints = true; 
}

void Polygon::calculateCenterAndPerimeter() {
	if (m_points->size() == 0) {
		m_center.set(0.0, 0.0);
		m_perimeter = 0.0;
	} else if (m_points->size() == 1) {
		osg::Vec2Array::iterator it = m_points->begin();
		osg::Vec2 point = (*it);
		m_center.set(point.x(), point.y());
		m_perimeter = 0.0;
	} else {
		double maxX = std::numeric_limits<double>::min();
		double minX = std::numeric_limits<double>::max();
		double maxY = std::numeric_limits<double>::min();
		double minY = std::numeric_limits<double>::max();
		for (osg::Vec2Array::iterator it = m_points->begin(); it != m_points->end(); ++it) {
			osg::Vec2 point = (*it);

			if ( point.x() > maxX ) {
				maxX =point.x();
			} else if ( point.x() < minX ) {
				minX = point.x();
			}

			if ( point.y() > maxY ) {
				maxY = point.y();
			} else if ( point.y() < minY ) {
				minY = point.y();
			}

			// Calculate center point of building
			double cx = ( minX + maxX ) / 2.0;
			double cy = ( minY + maxY ) / 2.0;

			m_center.set(cx, cy);
		}

		// Calculate perimeter
		m_perimeter = 0;
		for (size_t i = 0; i + 1 < m_points->size(); ++i) {
			m_perimeter += (m_points->at(i+1) - m_points->at(i)).length();
		}

		// Close polygon
		m_perimeter += (m_points->at(m_points->size() - 1) - m_points->at(0)).length();
	}
	m_dirtyPoints = false;
}

osg::Vec2 Polygon::center() {
	if (m_dirtyPoints) {
		calculateCenterAndPerimeter();
	}
	return m_center;
}

double Polygon::perimeter() {
	if (m_dirtyPoints) {
		calculateCenterAndPerimeter();
	}
	return m_perimeter;
}