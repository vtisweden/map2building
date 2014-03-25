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

void PolygonTree::addPolygon(osg::ref_ptr<Polygon> polygon) {
	// Check bound
	if (polygon->center().x() < m_minX) m_minX = polygon->center().x();
	if (polygon->center().x() > m_maxX) m_maxX = polygon->center().x();
	if (polygon->center().y() < m_minY) m_minY = polygon->center().y();
	if (polygon->center().x() > m_maxX) m_maxY = polygon->center().y();
	m_polygons.push_back(polygon);
}

void PolygonTree::setBucketSize(size_t bucketSize) {
	if (bucketSize == 0) {
		osg::notify(osg::WARN) << "Warning: Bucket size of 0 not allowed!" << std::endl;
	} else if (bucketSize < 25) {
		osg::notify(osg::WARN) << "Warning: Bucket size less than 25 is NOT recommended!" << std::endl;
		m_bucketSize = bucketSize;
	} else {
		m_bucketSize = bucketSize;
	}
}

void PolygonTree::balance() {
	// If tree node has more polygons than allowed bucket size
	if (m_polygons.size() > m_bucketSize) {
		// Split tree and balance nodes
		double w2 = (m_maxX - m_minX)/2.0;
		double h2 = (m_maxY - m_minY)/2.0;
		PolygonVectorIterator it;
		for (it = m_polygons.begin(); it != m_polygons.end(); ++it) {
			osg::ref_ptr<Polygon> polygon = (*it);
			
			if (polygon->center().x() > m_minX + w2) {
				if (polygon->center().y() > m_minY + h2) {
					// North East
					if (!m_northEast) {
						m_northEast = new PolygonTree;
					}
					m_northEast->addPolygon(polygon);
				}else {
					// South East
					if (!m_southEast) {
						m_southEast = new PolygonTree;
					}
					m_southEast->addPolygon(polygon);
				}
			} else {
				if (polygon->center().y() > m_minY + h2) {
					// North West
					if (!m_northWest) {
						m_northWest = new PolygonTree;
					}
					m_northWest->addPolygon(polygon);
				}else {
					// South West
					if (!m_southWest) {
						m_southWest = new PolygonTree;
					}
					m_southWest->addPolygon(polygon);
				}
			}
		}
		m_polygons.clear();
		// See if branches need balancing
		if (m_northEast) {
			m_northEast->balance();
		}
		if (m_northWest) {
			m_northWest->balance();
		}
		if (m_southEast) {
			m_southEast->balance();
		}
		if (m_southWest) {
			m_southWest->balance();
		}
	} else {
		osg::notify(osg::DEBUG_INFO) << "Number of polygon in leaf: " << m_polygons.size() << std::endl;
	}
}