module.exports = {
  config: {
    name: `testing`,
    category: "",
    description: "",
    aliases: [`testing_list`, "t_list"],
  },
  run: async (bot, message, args) => {
    
    let changeLog = "**CANARY Testing Commands v1.0**\nDescription: These commands are in testing and are not \"FINAL\"!!!\n```\ntesting_list```\n\n*The Yttrius Canary Program Copyright (C) 2021*";
    message.channel.send(changeLog);
  },
};
