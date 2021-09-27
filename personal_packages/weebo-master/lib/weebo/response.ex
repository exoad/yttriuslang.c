defmodule Weebo.Response do
  @moduledoc "Struct used to represent an XML-RPC response."
  defstruct error: nil, params: []

  @type t :: %Weebo.Response{error: map | nil, params: list}
end

defimpl Weebo.Formattable, for: Weebo.Response do
  @spec format(Weebo.Response.t) :: Weebo.xml_tree
  def format(response) do
    case response do
      %Weebo.Response{error: nil, params: params} ->
        mapped_params = Enum.map params, fn(param) ->
          {:param, [{:value, [Weebo.Formattable.format(param)]}]}
        end
        {:methodResponse, [{:params, mapped_params}]}
      %Weebo.Response{error: error, params: []} ->
        {:methodResponse, [{:fault, [{:value, [Weebo.Formattable.format(error)]}]}]}
    end
  end
end
