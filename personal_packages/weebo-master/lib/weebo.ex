defmodule Weebo do
  @moduledoc ~S"""
  Weebo is an [XML-RPC](http://wikipedia.org/wiki/XML-RPC) parser/formatter for
  Elixir, with full data-type support.

  Weebo can be combined with GenServer, Phoenix, HTTPoison (and others!) to create
  fully-featured XML-RPC clients & servers.

      request = Weebo.parse("<?xml version=\"1.0\"?><methodCall><methodName>math.sum</methodName><params><param><value><int>1</int></value></param><param><value><int>2</int></value></param><param><value><int>3</int></value></param></params></methodCall>")
      #=> %Weebo.Request{method: "math.sum", params: [1, 2, 3]}

      sum = Enum.sum(request.params)
      response = %Weebo.Response{error: nil, params: [sum]}
      Weebo.format(response)
      #=> "<?xml version=\"1.0\"?><methodResponse><params><param><value><int>6</int></value></param></params></methodResponse>"

  ## Data Type Mapping

  All the following data-types are supported, and will be automatically serialized
  in `format/1` and `parse/1`:

  | XMLRPC | Elixir |
  | -------|--------|
  | `<string>`    | Bitstring - `"string"` |
  | `<int>`       | Integer - `8` |
  | `<boolean>`   | Boolean - `true false `|
  | `<double>`    | Float - `6.3` |
  | `<array>`     | List - `[1, 2, 3]` |
  | `<struct>`    | Map - `%{key: "value"}`  |
  | `<dateTime.iso8601>`    | Tuple - `{{2015, 6, 7}, {16, 24, 18}}` |
  | `<nil>`    | Nil atom - `nil` |

  In addition, the following extra data-types are supported only in `parse/1`:

  | XMLRPC | Elixir |
  | -------|--------|
  | `<base64>`    | Bitstring - `"string"` (will decode the base64 first) |
  | `<i4>`       | Integer - `8` |

  """

  @typedoc ~S"""
  Tuple-representation of an XML element. This is the format Weebo uses internally
  during parsing/formatting.

  The first element is an atom that represents the XML node's name, and the second
  element is a list of child elements that follow the same pattern.

  ### Example

  The following bit of XML:

      <value>
        <string>Hello</string>
      </value>

  Would be represented as:

      {:value, [{:string, ["Hello"]}]}

  """
  @type xml_tree :: {atom, [String.t | tuple]}

  alias Weebo.XMLInterface, as: XML
  alias Weebo.Formattable

  @doc ~S"""
  Formats `%Weebo.Response` and `%Weebo.Request` into an XML-RPC string.

  ## Examples

      Weebo.format(%Weebo.Response{error: nil, params: [%{success: true}]})
      #=> "<?xml version=\"1.0\"?><methodResponse><params><param><value><struct><member><name>success</name><value><boolean>1</boolean></value></member></struct></value></param></params></methodResponse>"

  """
  @spec format(Weebo.Request.t | Weebo.Response.t) :: String.t
  def format(subject) do
    Formattable.format(subject)|>XML.from_tree|>XML.export
  end

  @doc ~S"""
  Parses an XML-RPC string into `%Weebo.Request` and `%Weebo.Response`.

  ## Examples

      Weebo.parse("<?xml version=\"1.0\"?><methodCall><methodName>math.sum</methodName><params><param><value><int>1</int></value></param><param><value><int>4</int></value></param></params></methodCall>")
      #=> %Weebo.Request{method: "math.sum", params: [1, 4]}

  """
  @spec parse(String.t) :: Weebo.Request.t | Weebo.Response.t
  def parse(string) when is_bitstring(string) do
    parsed = XML.parse(string)|>XML.to_tree
    case parsed do
      {:methodCall, [{:methodName, [name]}, {:params, params}]} ->
        %Weebo.Request{method: name, params: Enum.map(params, &cast/1)}
      {:methodResponse, [{:params, params}]} ->
        %Weebo.Response{error: nil, params: Enum.map(params, &cast/1)}
      {:methodResponse, [{:fault, [error]}]} ->
        %Weebo.Response{error: cast(error)}
    end
  end

  @doc false
  @spec cast(String.t) :: xml_tree
  @spec cast(xml_tree) :: any
  def cast(string) when is_bitstring(string) do
    XML.parse(string)|>XML.to_tree|>cast
  end
  def cast({:string, [string]}), do: string
  def cast({:string, []}), do: ""
  def cast({:boolean, ["1"]}), do: true
  def cast({:boolean, ["0"]}), do: false
  def cast({:int, [int]}), do: String.to_integer(int)
  def cast({:i4, [int]}), do: String.to_integer(int)
  def cast({:double, [double]}), do: String.to_float(double)
  def cast({:base64, [string]}), do: Base.decode64!(string)
  def cast({:"dateTime.iso8601", [value]}), do: :iso8601.parse(value)
  def cast({:array, [{:data, items}]}), do: Enum.map(items, &cast/1)
  def cast({:member, [{:name, [name]}, value]}), do: {String.to_atom(name), cast(value)}
  def cast({:struct, members}) do
    {_, casted} = Enum.map_reduce members, %{}, fn(member, acc) ->
      {name, value} = cast(member)
      {member, Map.put(acc, name, value)}
    end
    casted
  end
  def cast({:value, [value]}), do: cast(value)
  def cast({:param, [value]}), do: cast(value)
  def cast({:nil, []}), do: nil
  def cast(val) do
    {:unknown_type, val}
  end
end
