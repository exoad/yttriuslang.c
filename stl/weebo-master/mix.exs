defmodule Weebo.Mixfile do
  use Mix.Project

  def project do
    [app: :weebo,
     version: "0.1.2",
     elixir: "~> 1.0",
     name: "Weebo",
     description: "An XML-RPC parser/formatter for Elixir, with full support for datatype mapping!",
     package: [
       contributors: ["Steven Schobert"],
       licenses: ["MIT"],
       links: %{"GitHub" => "https://github.com/stevenschobert/weebo"},
       files: ~w(mix.exs README.md LICENSE.txt lib src)
     ],
     build_embedded: Mix.env == :prod,
     start_permanent: Mix.env == :prod,
     source_url: "https://github.com/stevenschobert/weebo",
     deps: deps]
  end

  def application do
    [applications: [:logger]]
  end

  defp deps do
    [{:ex_doc, "~> 0.7", only: :docs}]
  end
end
