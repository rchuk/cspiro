#define SDL_MAIN_HANDLED

#include <iostream>

#include "GL/glew.h"
#include "GL/GL.h"

#include "glm/gtc/type_ptr.hpp"

#include "application/application.hpp"
#include "graphics/shader.hpp"
#include "graphics/shader_program.hpp"
#include "spirograph/spiro_node.hpp"


class CSpiroApplication : public Application {
public:
    CSpiroApplication() : Application("cspiro", 1280, 720) {}

protected:
    virtual void on_start() override {
        create_shader_program();
        create_spiro();

        get_camera().set_position(glm::vec3(0.0f, 0.0f, 10.0f));
    }

    virtual void on_event(const SDL_Event& event) override {
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            // TODO: Only set flags. Move it to the frame loop
            case SDLK_a:
                get_camera().translate(glm::vec3(-camera_speed, 0.0f, 0.0f));
                break;
            case SDLK_d:
                get_camera().translate(glm::vec3(camera_speed, 0.0f, 0.0f));
                break;
            case SDLK_w:
                get_camera().translate(glm::vec3(0.0f, camera_speed, 0.0f));
                break;
            case SDLK_s:
                get_camera().translate(glm::vec3(0.0f, -camera_speed, 0.0f));
                break;
            case SDLK_MINUS:
                get_camera().set_zoom(glm::max(1.0f, get_camera().get_zoom() - camera_zoom_speed));
                break;
            case SDLK_EQUALS:
                get_camera().set_zoom(get_camera().get_zoom() + camera_zoom_speed);
                break;
            default:
                break;
            }
        }
    }

    virtual void on_update(float dt) override {
        spiro->tick(drawing_speed);
    }

    virtual void on_render() override {
        update_mvp_matrix();
        
        spiro->render();

        SDL_Delay(16); // TODO: Implement proper framerate limiting render loop
    }

private:
    constexpr static float camera_speed = 10.0f;
    constexpr static float camera_zoom_speed = 5.0f;

    constexpr static float drawing_speed = 0.01f;

    std::unique_ptr<ShaderProgram> shader_program;

    std::unique_ptr<Spirograph> spiro;

    void create_shader_program() {
        const std::unique_ptr<Shader> vertex_shader = Shader::load(GL_VERTEX_SHADER, "../shaders/base_vertex.vert");
        const std::unique_ptr<Shader> fragment_shader = Shader::load(GL_FRAGMENT_SHADER, "../shaders/base_fragment.frag");
        shader_program = ShaderProgram::create({ *vertex_shader, *fragment_shader });
        
        glUseProgram(shader_program->get_handle());
    }

    void create_spiro() {
        spiro = SpiroNode::root()
            .set_radius(64.0f)
            .attach(SpiroNode::inside()
                .set_radius(6.0f)
                .set_rotation_speed(5.0f)
                .set_pen(Pen()
                    .set_color(glm::vec3(0.0f, 0.5f, 0.7f))
                )
                .attach(SpiroNode::inside()
                    .set_radius(2.0f)
                    .set_pen(Pen()
                       .set_color(glm::vec3(0.5f, 0.2f, 0.6f))
                    )
                )
            )
            .attach(SpiroNode::inside()
                .set_radius(12.0f)
                .set_rotation_speed(2.5f)
                .set_pen(Pen()
                    .set_color(glm::vec3(1.0f, 0.0f, 0.0f))
                    .set_stroke_width(0.2f)
                )
            )
            .attach(SpiroNode::outside()
                .set_radius(1.0f)
                .set_rotation_speed(2.5f)
                .set_pen(Pen()
                   .set_color(glm::vec3(0.6f, 0.8f, 0.0f))
                    .set_stroke_width(0.2f)
                )
                .attach(SpiroNode::outside()
                    .set_radius(1.0f)
                    .set_rotation_speed(10.0f)
                    .set_pen(Pen()
                        .set_color(glm::vec3(0.25f, 0.25f, 0.25f))
                    )
                )
            )
            .build(glm::vec2(0.0f, 0.0f));
    }

    void update_mvp_matrix() {
        glUniformMatrix4fv(glGetUniformLocation(shader_program->get_handle(), "model_matrix"), 1, GL_FALSE, glm::value_ptr(get_model_matrix()));
        glUniformMatrix4fv(glGetUniformLocation(shader_program->get_handle(), "view_matrix"), 1, GL_FALSE, glm::value_ptr(get_camera().get_view_matrix()));
        glUniformMatrix4fv(glGetUniformLocation(shader_program->get_handle(), "projection_matrix"), 1, GL_FALSE, glm::value_ptr(get_camera().get_projection_matrix()));
    }

    // TODO: Create transform object (individual per rendering object) and move it there
    glm::mat4 get_model_matrix() {
        return glm::mat4(1.0f);
    }
};

int main() {
    CSpiroApplication app;
    if (!app.run())
        return 1;

    return 0;
}
