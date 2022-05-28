#include <Next.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui\imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

class ExampleLayer : public Next::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		m_VertexArray.reset(Next::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		};

		Next::Ref<Next::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Next::VertexBuffer::Create(vertices, sizeof(vertices)));

		Next::BufferLayout layout = {
			{Next::ShaderDataType::Float3,"a_Position"},
			{Next::ShaderDataType::Float4,"a_Color"},
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Next::Ref<Next::IndexBuffer> indexBuffer;
		indexBuffer.reset(Next::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(Next::VertexArray::Create());

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Next::Ref<Next::VertexBuffer> squareVB;
		squareVB.reset(Next::VertexBuffer::Create((squareVertices), sizeof(squareVertices)));

		squareVB->SetLayout({
				{Next::ShaderDataType::Float3,"a_Position"},
				{Next::ShaderDataType::Float2,"a_TexCoord"},
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Next::Ref<Next::IndexBuffer> squareIB;
		squareIB.reset(Next::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader = Next::Shader::Create("VertexPosColor",vertexSrc, fragmentSrc);

		//Shader::Create("assets/shaders/Texture.glsl");

		std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string flatColorShaderfragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color,1.0);
			}
		)";

		m_FlatColorShader = Next::Shader::Create("FlatColor",flatColorShaderVertexSrc, flatColorShaderfragmentSrc);

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = Next::Texture2D::Create("assets/textures/star.png");

		std::dynamic_pointer_cast<Next::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Next::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);

	}

	void OnUpdate(Next::Timestep timestep) override
	{
		if (Next::Input::IsKeyPressed(NX_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * timestep;

		else if (Next::Input::IsKeyPressed(NX_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * timestep;

		if (Next::Input::IsKeyPressed(NX_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * timestep;

		else if (Next::Input::IsKeyPressed(NX_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * timestep;

		if (Next::Input::IsKeyPressed(NX_KEY_A))
			m_CameraRotation -= m_CameraRotationSpeed * timestep;

		else if (Next::Input::IsKeyPressed(NX_KEY_D))
			m_CameraRotation += m_CameraRotationSpeed * timestep;


		Next::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Next::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRoation(m_CameraRotation);

		Next::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		glm::vec4 redColor(0.8f, 0.2f, 0.3f, 1.0f);
		glm::vec4 blueColor(0.2f, 0.3f, 0.8f, 1.0f);

		/*	Next::MaterialRef material = new Next::Material(m_FlatColorShader);
			Next::MaterialInstanceRef mi = new Next::MaterialInstance(material);

			material->Set("u_Color", redColor);
			squareMesh->SetMaterial(material);*/

		std::dynamic_pointer_cast<Next::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Next::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);
		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Next::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);

				//material
				//Next::Renderer::Submit(mi, m_SquareVA, transform);
			}
		}

		auto textureShader = m_ShaderLibrary.Get("Texture");
		m_Texture->Bind();
		Next::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));


		//Triangle
		//Next::Renderer::Submit(m_Shader, m_VertexArray);

		Next::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Next::Event& event) override
	{

	}

private:
	Next::ShaderLibrary m_ShaderLibrary;
	Next::Ref<Next::Shader> m_Shader;
	Next::Ref<Next::Shader> m_FlatColorShader;

	Next::Ref<Next::VertexArray> m_VertexArray;
	Next::Ref<Next::VertexArray> m_SquareVA;

	Next::Ref<Next::Texture2D> m_Texture;

	Next::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 180.0f;

	glm::vec3 m_SquareColor = { 0.2f,0.3f,0.8f };
};

class Sandbox : public Next::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

Next::Application* Next::CreateApplication()
{
	return new Sandbox();
}