#pragma once

#include <vector>
#include <memory>
#include <optional>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

#include "circle_transform.hpp"
#include "graphics/line_strip.hpp"


class SpirographPen {
public:
    SpirographPen(CircleTransform transform, glm::vec3 color)
        : transform(transform), color(color), strip_renderer(LineStripRenderer::create()) {}

    void tick(const CircleTransform& parent, glm::vec2 parent_cart_pos);
    void render() const;

private:
    CircleTransform transform;
    glm::vec3 color;

    std::unique_ptr<LineStripRenderer> strip_renderer;
};

class SpirographNode {
public:
    SpirographNode(CircleTransform transform, float rotation_speed, std::optional<SpirographPen> pen, std::vector<SpirographNode> children)
        : transform(transform), rotation_speed(rotation_speed), pen(std::move(pen)), children(std::move(children))
    {}

    void render() const;
    void tick(float delta, CircleTransform& parent_transform, glm::vec2 parent_cart_pos);

private:
    CircleTransform transform;
    float rotation_speed;

    std::optional<SpirographPen> pen;
    std::vector<SpirographNode> children;
};

class Spirograph {
public:
    Spirograph(glm::vec2 position, float radius, std::vector<SpirographNode> children)
        : position(position), transform(CircleTransform(PolarCoords(0.0f, 0.0f), radius, 0.0f)), children(std::move(children))
    {}

    void render() const;
    void tick(float delta);

private:
    glm::vec2 position;
    CircleTransform transform;
    std::vector<SpirographNode> children;
};

class Pen {
public:
    Pen() {}

    SpirographPen build(const CircleTransform& parent_transform) const;

    Pen& set_position(float new_radius_mult);
    Pen& set_stroke_width(float new_stroke_width);
    Pen& set_color(glm::vec3 new_color);

private:
    float radius_mult = 1.0f;
    float stroke_width = 0.1f;
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
};

class SpiroNode {
public:
    enum class Attachment: uint8_t {
        Inside = 0,
        Outside
    };

    static SpiroNode root();
    std::unique_ptr<Spirograph> build(glm::vec2 position);

    static SpiroNode inside();
    static SpiroNode outside();

    SpiroNode& attach(SpiroNode child);
    SpiroNode& set_pen(Pen new_pen);
    SpiroNode& set_radius(float new_radius);

    SpiroNode& set_rotation_speed(float new_rotation_speed);
    SpiroNode& set_starting_angle(float new_starting_angle);

private:
    Attachment attachment;
    float radius = 1.0f;
    float rotation_speed = 1.0f;
    
    float starting_angle = 0.0f;
    float starting_rotation = 0.0f;

    std::optional<Pen> pen;
    std::vector<SpiroNode> children;

private:
    SpiroNode(Attachment attachment) : attachment(attachment) {}

    std::vector<SpirographNode> build_node_children() const;
    SpirographNode build_node(const SpiroNode& parent) const;
};