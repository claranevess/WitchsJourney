import sys
import google.generativeai as genai

PROMPT = (
    "gemini voce é um randomizador de atividades voce vai gerar um power up na minha tela. "
    "Preciso que você me indique uma posição entre 1280(x) e 780(y) e um número de 1 a 3. "
    "Quero que voce me responda em apenas uma linha somente com os inteiros separados por um espaço."
)

API_KEY = "AIzaSyDhtRXrkpyrkvo643lTiiZf__WYGdw8mLQ"

def requisicao() -> str | None:
    """
    Envia um prompt para a API do Gemini e retorna a resposta textual.
    Retorna None em caso de erro, imprimindo o erro em stderr.
    """
    if not API_KEY or API_KEY == "SUA_CHAVE_AQUI":
        print("API_KEY não definida ou inválida.", file=sys.stderr)
        return None

    try:
        genai.configure(api_key=API_KEY)

        generation_config = {
            "temperature": 1.0,
            "top_p": 1,
            "top_k": 1,
            "max_output_tokens": 60,
        }

        model = genai.GenerativeModel(
            model_name="gemini-1.5-flash",
            generation_config=generation_config,
        )

        response = model.generate_content(PROMPT)
        return response.text.strip()

    except Exception as e:
        print(f"Erro na requisição: {e}", file=sys.stderr)
        return None

if __name__ == "__main__":
    resposta_bruta = requisicao()

    if resposta_bruta:
        print(resposta_bruta)
    else:
        sys.exit(1)