#include "Camera.hpp"
#include <iostream>
#include <GLFW/glfw3.h>

namespace gps {

    //Camera constructor
    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp) {
        //TODO
        this->cameraPosition = cameraPosition;
        this->cameraTarget = cameraTarget;
        this->cameraFrontDirection = glm::normalize(cameraPosition - cameraTarget);
        this->cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, cameraUp));
        this->cameraUpDirection = cameraUp;
        this->sensitivity = 0.32f;
        this->yaw = -90.0f;
        this->pitch = 0.0f;
        this->counter = 0;
        this->maxCounter = 20;
        this->isCrouched = false;
    }

    //return the view matrix, using the glm::lookAt() function
    glm::mat4 Camera::getViewMatrix() {
        //TODO
        return glm::lookAt(cameraPosition, cameraPosition + cameraFrontDirection, cameraUpDirection);
        //return glm::mat4();
    }

    //update the camera internal parameters following a camera move event
    void Camera::move(MOVE_DIRECTION direction, float speed) {
        //TODO

        //movement to immitate the camera movement on the Y axis
        counter++;

        if (counter <= maxCounter/2) {
            cameraPosition.y += 0.02f;

        }
        else {
            cameraPosition.y -= 0.02f;
        }

        if (counter == maxCounter) {
            counter = 0;
        }

        glm::vec3 movement = glm::vec3(0.0f);

        switch (direction)
        {
        case gps::MOVE_FORWARD:
            movement += cameraFrontDirection * speed;
            break;
        case gps::MOVE_BACKWARD:
            movement -= cameraFrontDirection * speed;
            break;
        case gps::MOVE_RIGHT:
            movement += cameraRightDirection * speed;
            break;
        case gps::MOVE_LEFT:
            movement -= cameraRightDirection * speed;
            break;
        case gps::MOVE_UP:
            movement += cameraUpDirection * speed;
			break;
        case gps::MOVE_DOWN:
			movement -= cameraUpDirection * speed;
			break;
        }

        
        //to move only on the XZ plane
        movement.y = 0.0f;

        cameraPosition += movement;
    }

    //update the camera internal parameters following a camera rotate event
    //yaw - camera rotation around the y axis
    //pitch - camera rotation around the x axis
    void Camera::rotate(float pitch, float yaw) {
        //TODO

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        cameraFrontDirection = glm::normalize(front);
        cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, cameraUpDirection));
    }


    void Camera::processMouseMovement(float xOffset, float yOffset, bool constrainPitch) {
        xOffset *= sensitivity;
        yOffset *= sensitivity;

        yaw += xOffset;
        pitch += yOffset;

        // Constrain pitch to avoid camera flipping
        if (constrainPitch) {
            if (pitch > 89.0f) pitch = 89.0f;
            if (pitch < -89.0f) pitch = -89.0f;
        }

        // Update camera orientation
        rotate(pitch, yaw);
        
    }

    //get camera position
    glm::vec3 Camera::getCameraPosition() {

		return cameraPosition;
	}


    //crouch
    void Camera::crouch() {
        if (isCrouched) {
			cameraPosition.y += 0.6f;
			isCrouched = false;
		}
        else {
			cameraPosition.y -= 0.6f;
			isCrouched = true;
		}
    }

    

}