module.exports = {
  config: {
    name: `testing_about`,
    category: "",
    description: "",
    aliases: ["t_about"],
  },
  run: async (bot, message, args) => {
    let changeLog =
      '**CANARY Program (C) 2021**\nThe Canary Program is an "Beta test" for `Yttrius`. This means it incorporates all the commands that Yttrius has. Changes that has gone through `Pre-Bed` will be tested here. This is the middle development bot, with the front-end development being `Pre-bed` which is not at all user-friendly and does not include Yttrius commands.';
    message.channel.send(changeLog);
  },
};
