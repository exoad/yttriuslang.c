defmodule WeeboTest do
  use ExUnit.Case

  def unknown_type() do
    "<foo>bar</foo>"
  end

  def string_type(string) do
    "<string>#{string}</string>"
  end

  def empty_string_type() do
    "<string></string>"
  end

  def boolean_type(true) do
    "<boolean>1</boolean>"
  end
  def boolean_type(false) do
    "<boolean>0</boolean>"
  end

  def int_type(num) do
    "<int>#{num}</int>"
  end

  def i4_type(num) do
    "<i4>#{num}</i4>"
  end

  def double_type(num) do
    "<double>#{num}</double>"
  end

  def nil_type() do
    "<nil></nil>"
  end

  def nil_type_short() do
    "<nil/>"
  end

  def base64_type(string) do
    "<base64>#{Base.encode64(string)}</base64>"
  end

  def param_type(string) do
    "<param>#{string_type(string)}</param>"
  end

  def date_type(date) do
    "<dateTime.iso8601>#{:iso8601.format(date)}</dateTime.iso8601>"
  end

  def member_type(name: name, type: type, value: val) do
    typed_value = apply(__MODULE__, String.to_atom("#{type}_type"), [val])
    "<member><name>#{name}</name><value>#{typed_value}</value></member>"
  end

  def array_type(list) when is_list(list) do
    {_, typed} = Enum.map_reduce list, "<array><data>", fn({type, value}, acc) ->
      typed_value = apply(__MODULE__, String.to_atom("#{type}_type"), [value])
      {{type, value}, "#{acc}<value>#{typed_value}</value>"}
    end
    "#{typed}</data></array>"
  end

  def struct_type(list) when is_list(list) do
    {_, typed} = Enum.map_reduce list, "<struct>", fn({name, type, value}, acc) ->
      member = member_type(name: name, type: type, value: value)
      {{name, type, value}, "#{acc}#{member}"}
    end
    "#{typed}</struct>"
  end

  def sample_call() do
    """
    <?xml version=\"1.0\"?>
    <methodCall>
      <methodName>weebo.sample_call</methodName>
      <params>
        <param>
          <value><int>40</int></value>
        </param>
        <param>
          <value><boolean>0</boolean></value>
        </param>
      </params>
    </methodCall>
    """
  end

  def sample_response() do
    """
    <?xml version=\"1.0\"?>
    <methodResponse>
      <params>
        <param>
          <value><string>Weebo</string></value>
        </param>
      </params>
    </methodResponse>
    """
  end

  def sample_fault() do
    """
    <?xml version=\"1.0\"?>
    <methodResponse>
      <fault>
        <value>
          <struct>
            <member>
              <name>faultCode</name>
              <value><int>4</int></value>
            </member>
            <member>
              <name>faultString</name>
              <value><string>Too many parameters.</string></value>
            </member>
          </struct>
        </value>
      </fault>
    </methodResponse>
    """
  end

  def strip_xml(xml) do
    String.replace(xml, ~r/[\n\s]*/, "", global: true)
  end

  test "#format" do
    assert Weebo.format(%Weebo.Request{method: "weebo.sample_call", params: [40, false]})|>strip_xml == strip_xml(sample_call)
    assert Weebo.format(%Weebo.Response{error: nil, params: ["Weebo"]})|>strip_xml == strip_xml(sample_response)
    assert Weebo.format(%Weebo.Response{error: %{faultCode: 4, faultString: "Too many parameters."}, params: []})|>strip_xml == strip_xml(sample_fault)
  end

  test "#parse" do
    assert Weebo.parse(sample_call) == %Weebo.Request{method: "weebo.sample_call", params: [40, false]}
    assert Weebo.parse(sample_response) == %Weebo.Response{error: nil, params: ["Weebo"]}
    assert Weebo.parse(sample_fault) == %Weebo.Response{error: %{faultCode: 4, faultString: "Too many parameters."}, params: []}
  end

  test "#cast" do
    assert string_type("xmlrpc")|>Weebo.cast == "xmlrpc"
    assert empty_string_type()|>Weebo.cast == ""

    assert base64_type("xmlrpc")|>Weebo.cast == "xmlrpc"

    assert boolean_type(true)|>Weebo.cast == true
    assert boolean_type(false)|>Weebo.cast == false

    assert int_type(5)|>Weebo.cast == 5
    assert i4_type(5)|>Weebo.cast == 5

    assert double_type(12.5)|>Weebo.cast == 12.5

    assert member_type(name: "foo", type: :string, value: "bar")|>Weebo.cast == {:foo, "bar"}

    assert array_type([{:boolean, true}, {:string, "hello"}, {:int, 40}])|>Weebo.cast == [true, "hello", 40]

    assert struct_type([{:foo, :string, "bar"}, {:number, :int, 40}, {:online, :boolean, false}])|>Weebo.cast == %{foo: "bar", number: 40, online: false}

    assert Weebo.cast(nil_type) == nil
    assert Weebo.cast(nil_type_short) == nil

    timestamp = :calendar.universal_time
    assert date_type(timestamp)|>Weebo.cast == timestamp

    assert param_type("test")|>Weebo.cast == "test"
  end
end
